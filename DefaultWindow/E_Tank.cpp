#include "pch.h"
#include "E_Tank.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CBulletEffect.h"
#include "CCollisionMgr.h"
#include "CSoundMgr.h"

void E_Tank::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD);

	m_eObjID = OT_Tank;
	m_tStat = { 150.f, 150.f, 30, 1, 224, 1.8f, 625 , DF_LAGE, AT_EXPLOSIVE };

	m_iAttackFrame = 1;
	m_eInput = IP_Chase;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_bIsEnemy = true;
}

int E_Tank::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"TankDead.mp3", 0.3f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CSCVDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));

		return OBJ_DEAD;
	}

	KeyInput();
	SiegeMode();
	UnSiegeMode();
	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void E_Tank::Late_Update()
{
	Change_Motion();

	if (m_bSiegeMode_Anime) return;
	if (m_bSiegeMode) return;
	CUnit::Move_Frame();
	MoveBody_Frame();

	if (dwAttackCoolTime + 1700 < GetTickCount64())
	{
		m_isAttack = false;
		dwAttackCoolTime = GetTickCount64();
	}
}

void E_Tank::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hBodyDC = CBmpMgr::Get_Instance()->Find_Image(L"TankBody_Blue");
	HDC		hHeadDC = CBmpMgr::Get_Instance()->Find_Image(L"TankHead_Blue");
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_4_E");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 25,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 30,
			80,			// 복사 받을 이미지의 가로, 세로
			80,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			80,										// 복사할 이미지의 가로, 세로
			80,
			RGB(255, 0, 255));		// 제거할 색상
	}

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hBodyDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tBodyFram.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY - 5,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hHeadDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eAttackDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상

}

void E_Tank::Release()
{
}

void E_Tank::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			if (!m_bSiegeMode)
			{
				Frame_Init_Body(0, 0, 200);
				Frame_Init_Head(0, 0, 200);
			}
			else
			{
				m_tBodyFram.iCurCount = 3;
				m_eDir = (DIRECTION)5;
				Frame_Init_Head(2, 2, 200);
			}
			break;

		case STATE_MOVE:
			Frame_Init_Body(1, 2, 200);
			Frame_Init_Head(0, 0, 200);
			break;

		case STATE_ATTACK:
			if (!m_bSiegeMode)
			{
				Frame_Init_Body(0, 0, 200);
				Frame_Init_Head(0, 0, 200);
			}
			else
			{
				m_tBodyFram.iCurCount = 3;
				m_eDir = (DIRECTION)5;
				Frame_Init_Head(2, 2, 200);
			}
			break;

		case STATE_SHOOT:
			if (!m_bSiegeMode)
			{
				Frame_Init_Body(0, 0, 200);
				Frame_Init_Head(0, 1, 600);
			}
			else
			{
				m_tBodyFram.iCurCount = 3;
				m_eDir = (DIRECTION)5;
				Frame_Init_Head(2, 2, 200);


			}
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void E_Tank::KeyInput()
{
	return;
}

void E_Tank::AttackToEnemy(CObj* _Enemey)
{
	m_eAttackDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT)
		m_eCurState = STATE_SHOOT;
	else
		m_eCurState = STATE_ATTACK;

	if (!m_bSiegeMode)
	{
		if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64())
		{
			if (m_tFrame.iCurCount == m_iAttackFrame && !m_isAttack)
			{
				DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
				ATTACKID Attack_id = m_tStat.m_eAttackID;
				float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));

				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CTankHit>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
				CSoundMgr::Get_Instance()->PlaySFX(L"TankAttack1.mp3", 0.8f);
				_Enemey->Add_Stat_hp(-Damge);
				m_isAttack = true;

				m_AttackTime = GetTickCount64();
			}
		}
	}
	else
	{
		if (m_AttackTime + _Enemey->Get_Stat()->Colldown + 2000 < GetTickCount64())
		{
			CSoundMgr::Get_Instance()->PlaySFX(L"TankSiegeAttak.mp3", 0.8f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CSiegeTankHit_E>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
			m_AttackTime = GetTickCount64();
		}
	}

}

void E_Tank::Hold()
{
	CObj* Enemy = nullptr;
	if (!m_bSiegeMode)
	{
		if ((Enemy = CCollisionMgr::Collision_RangeChack_Attack(this, *m_pMonsterList, *m_pBuildList_E, m_tStat.m_iRange)) == nullptr)
		{
			m_eCurState = STATE_IDLE;
		}
		else
		{
			AttackToEnemy(Enemy); // 실제 데미지 주는 코드
		}
	}
	else
	{
		if ((Enemy = CCollisionMgr::Collision_RangeChack_Attack(this, *m_pMonsterList, *m_pBuildList_E, m_tStat.m_iRange + 160.f)) == nullptr)
		{
			m_eCurState = STATE_IDLE;
		}
		else
		{
			AttackToEnemy(Enemy); // 실제 데미지 주는 코드
		}
	}
}

void E_Tank::Move_toNext()
{
	Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
	Pos _pos = _path[m_iPathIndex];

	if (m_iPathIndex == 0)
	{
		CMapMgr::Get_Instance()->SetTileType(_pos, 0);
		m_iPathIndex = 1;
		return;
	}

	if (_now == _pos)
	{
		++m_iPathIndex;
	}
	else
	{
		Pos dir = (_pos - _now);
		for (int i = 0; i < DIR_END; i++)
		{
			if (dir == MoveFront[i])
			{
				m_eDir = (DIRECTION)i;
				m_eAttackDir = (DIRECTION)i;
				break;
			}
		}

		// 이동
		if (CCollisionMgr::Collision_RangeChack_bool(this, *m_pUnitList, 50.f))
		{
			m_eCurState = STATE_IDLE;
		}
		else
		{
			// 단위 벡터로 수정?
			fPOINT point = Nomalization(MoveFront[m_eDir]);

			m_eCurState = STATE_MOVE;
			m_tInfo.fX += m_tStat.m_fSpeed * point.x;
			m_tInfo.fY += m_tStat.m_fSpeed * point.y;
		}
	}

}

void E_Tank::Update_State()
{
	switch (m_eInput)
	{
	case IP_MOVE:
		if (m_bSiegeMode)
		{
			m_eInput = IP_HOLD;
			return;
		}
		Move();
		break;
	case IP_ATTACK:
		if (m_bSiegeMode) return;
		Attack();
		break;
	case IP_HOLD:
		Hold();
		break;
	case IP_STOP:
		Stop();
		break;
	case IP_Chase:
		ChaseUnit();
		break;
	case IP_END:
		break;
	default:
		break;
	}
}

void E_Tank::MoveBody_Frame()
{
	if (m_tBodyFram.dwTime + m_tBodyFram.dwSpeed < GetTickCount64())
	{
		++m_tBodyFram.iCurCount;

		if (m_eCurState == STATE_ATTACK && m_tBodyFram.iCurCount > m_tBodyFram.iFrameEnd)
		{
			m_eCurState = STATE_SHOOT;
			m_tBodyFram.iCurCount = m_tBodyFram.iFrameStart;
		}
		else if (m_tBodyFram.iCurCount > m_tBodyFram.iFrameEnd)
			m_tBodyFram.iCurCount = m_tBodyFram.iFrameStart;


		m_tBodyFram.dwTime = GetTickCount64();
	}
}

void E_Tank::SiegeMode()
{
	if (!m_bSiegeMode_Anime) return;
	if (m_bSiegeMode) return;

	m_tBodyFram.iCurCount = 3;
	m_tFrame.iCurCount = 3;
	m_eDir = (DIRECTION)5;
	m_eAttackDir = (DIRECTION)4;
	m_eObjID = OT_SiegeTank;
	m_bSiegeMode = true;
	m_bSiegeMode_Anime = false;
	m_eInput = IP_HOLD;
}

void E_Tank::UnSiegeMode()
{
	if (!m_bSiegeMode_Anime) return;
	if (!m_bSiegeMode) return;

	m_eDir = (DIRECTION)0;
	m_eAttackDir = (DIRECTION)0;
	m_eObjID = OT_Tank;
	m_bSiegeMode = false;
	m_bSiegeMode_Anime = false;
	m_eInput = IP_HOLD;
}

void E_Tank::Frame_Init_Head(int start, int end, int time)
{
	m_tFrame.iFrameStart = start;
	m_tFrame.iFrameEnd = end;
	m_tFrame.iCurCount = start;
	m_tFrame.dwSpeed = time;
	m_tFrame.dwTime = GetTickCount64();
}

void E_Tank::Frame_Init_Body(int start, int end, int time)
{
	m_tBodyFram.iFrameStart = start;
	m_tBodyFram.iFrameEnd = end;
	m_tBodyFram.iCurCount = start;
	m_tBodyFram.dwSpeed = time;
	m_tBodyFram.dwTime = GetTickCount64();
}
