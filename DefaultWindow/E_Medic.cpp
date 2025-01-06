#include "pch.h"
#include "E_Medic.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CSoundMgr.h"

void E_Medic::Initialize()
{

	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD);

	m_pImgKey = L"Medic_Blue";
	m_eObjID = OT_Medic;
	m_tStat = { 60.f, 60.f, 0, 1, 64, 1.8f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 10;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_bIsEnemy = true;

	A_GroundPos = { 10,10 };
	Astar(A_GroundPos);
	m_eInput = IP_ATTACK;
}

int E_Medic::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"MedicDead.mp3", 0.5f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CMedicDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		

		return OBJ_DEAD;
	}

	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void E_Medic::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void E_Medic::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Medic_Blue");
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_0_E");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 15,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 25,
			32,			// 복사 받을 이미지의 가로, 세로
			32,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			32,										// 복사할 이미지의 가로, 세로
			32,
			RGB(255, 0, 255));		// 제거할 색상
	}

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));		// 제거할 색상

}

void E_Medic::Release()
{
}

void E_Medic::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_MOVE:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iCurCount = 1; 
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 7;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iCurCount = 7;
			m_tFrame.dwSpeed = 300;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 7;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iCurCount = 7;
			m_tFrame.dwSpeed = 300;
			m_tFrame.dwTime = GetTickCount64();
			break;


		}

		m_ePreState = m_eCurState;
	}
}

void E_Medic::KeyInput()
{
}

void E_Medic::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	m_eCurState = STATE_ATTACK;

	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() &&
		m_tFrame.iCurCount == m_iAttackFrame)
	{
		_Enemey->Add_Stat_hp(10);

		m_AttackTime = GetTickCount64();
	}
}

void E_Medic::Move()
{
	if (m_iPathIndex < _path.size())
	{
		Move_toNext();
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eInput = IP_HEAL;
	}
}


void E_Medic::Update_State()
{
	switch (m_eInput)
	{
	case IP_MOVE:
		Move();
		break;
	case IP_ATTACK:
		Attack();
		break;
	case IP_HOLD:
		Hold();
		break;
	case IP_STOP:
		Stop();
		break;
	case IP_HEAL:
		HealUnit();
	case IP_END:
		break;
	default:
		break;
	}
}

void E_Medic::HealUnit()
{
	CObj* unit(nullptr);

	if ((unit = CCollisionMgr::Collision_RangeChack_Heal(this, *m_pUnitList, 256.f)) != nullptr)
	{
		if (preUint != unit)
		{
			Astar(CCollisionMgr::Collision_RangePos(this, unit, 32.f));
			preUint = unit;
		}

		if (m_iPathIndex < _path.size())
		{
			Move_toNext(); // 가까히 다가가기
		}
		else if (m_iPathIndex == _path.size())
		{
			if (CCollisionMgr::Collision_Range_Bool(this, unit, 98.f))
			{
				// 힐
				AttackToEnemy(unit);
			}
			else
			{
				m_eCurState = STATE_IDLE;
				preUint = nullptr;
			}
		}
	}
	else
	{
		m_eCurState = STATE_IDLE;
		return;
	}
}

void E_Medic::Attack()
{
	CObj* unit(nullptr);

	if ((unit = CCollisionMgr::Collision_RangeChack_Heal(this, *m_pUnitList, m_tStat.m_iRange + 64.f)) != nullptr)
	{
		if (unit == this) return;

		if (CCollisionMgr::Collision_Range_Bool(this, unit, m_tStat.m_iRange)) // 충돌 범위 내
		{
			AttackToEnemy(unit);
		}
		else
		{
			if (preUint != unit)
			{
				Astar(CCollisionMgr::Collision_RangePos(this, unit, m_tStat.m_iRange - 32.f));
				preUint = unit;
			}

			if (m_iPathIndex < _path.size())
			{
				Move_toNext();
			}
			else if (m_iPathIndex == _path.size())
			{
				if (CCollisionMgr::Collision_Range_Bool(this, unit, m_tStat.m_iRange)) // 충돌 범위 내
				{
					AttackToEnemy(unit);
				}
				else
				{
					Astar(CCollisionMgr::Collision_RangePos(this, unit, m_tStat.m_iRange - 32.f));
					return;
				}

			}
		}

	}
	else
	{
		if (m_iPathIndex < _path.size())
		{
			Move_toNext();
		}
		else if (m_iPathIndex == _path.size())
		{

			if (preUint != nullptr)
			{
				Astar(A_GroundPos); // 새로운 목표 위치로 이동
				preUint = nullptr;
			}
			else
				m_eCurState = STATE_IDLE;
		}
	}
}
