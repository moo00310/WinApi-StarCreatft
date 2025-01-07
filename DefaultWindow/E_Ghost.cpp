#include "pch.h"
#include "E_Ghost.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CBulletEffect.h"
#include "CSoundMgr.h"

void E_Ghost::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD);

	m_pImgKey = L"Ghost_Blue";
	m_eObjID = OT_Ghost;
	m_tStat = { 45.f, 45.f, 10, 0, 224, 1.8f, 625 , DF_SAMLL, AT_CONCUSSIVE };

	m_iAttackFrame = 12;
	m_eInput = IP_Chase;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_bIsEnemy = true;
}

int E_Ghost::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"Ghost_Dead .mp3", 0.8f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));

		return OBJ_DEAD;
	}

	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void E_Ghost::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void E_Ghost::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
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
		RGB(0, 255, 0));		// 제거할 색상

}

void E_Ghost::Release()
{
}

void E_Ghost::Change_Motion()
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
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 9;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iCurCount = 9;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 11;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iCurCount = 11;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void E_Ghost::KeyInput()
{
}

void E_Ghost::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT)
		m_eCurState = STATE_SHOOT;
	else
		m_eCurState = STATE_ATTACK;

	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() &&
		m_tFrame.iCurCount == m_iAttackFrame)
	{
		DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
		ATTACKID Attack_id = m_tStat.m_eAttackID;
		float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));

		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostHit>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
		CSoundMgr::Get_Instance()->PlaySFX(L"GhostAttack.mp3", 0.8f);
		_Enemey->Add_Stat_hp(-Damge);

		m_AttackTime = GetTickCount64();
	}
}
