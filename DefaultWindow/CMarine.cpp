#include "pch.h"
#include "CMarine.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CMapMgr.h"
#include "CSoundMgr.h"
#include "CBulletEffect.h"
#include "CGameMgr.h"
#include "CSoundMgr.h"
#include "CCollisionMgr.h"

CMarine::CMarine(): m_bIsSteamPack(false), m_CoolDown(0)
{
}

CMarine::~CMarine()
{
	Release();
}

void CMarine::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

    m_pImgKey = L"Marine";
	m_eObjID = OT_Marine;
    m_tStat = { 40.f, 40.f, 6, 0, 128, 1.8f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 14;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	A_GroundPos.x = (int)m_tInfo.fX / 32;
	A_GroundPos.y = (int)m_tInfo.fY / 32;
}

int CMarine::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->Stop_SFX(SOUND_SFX);
		CSoundMgr::Get_Instance()->PlaySFX(L"Marine_Dead_1.mp3", 0.5f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,CAbstractFactory<CMarineDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));


		return OBJ_DEAD;
	}
	
	KeyInput();
	Update_State();

	if (CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_SightUp)) 
		m_tStat.m_iRange = 150;

	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMarine::Late_Update()
{
	Change_Motion();
	StreamPackCoolDown();
	CUnit::Move_Frame();
}

void CMarine::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_0");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 10,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 18,
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
		RGB(255, 255, 0));		// 제거할 색상

	
}

void CMarine::Release()
{
}

void CMarine::Change_Motion()
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
			m_tFrame.iFrameStart = 11;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iCurCount = 11;
			m_tFrame.dwTime = GetTickCount64();
			if (!m_bIsSteamPack)
				m_tFrame.dwSpeed = 100;
			else
				m_tFrame.dwSpeed = 55;
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 13;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iCurCount = 13;
			m_tFrame.dwTime = GetTickCount64();
			if(!m_bIsSteamPack)
				m_tFrame.dwSpeed = 100;
			else
				m_tFrame.dwSpeed = 55;
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CMarine::KeyInput()
{

	if (!m_bSelect) return;

	// 스팀 팩
	if (CKeyMgr::Get_Instance()->Key_Down('T'))
	{
		if (!CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_Streampack)) return;

		m_bIsSteamPack = true;
		m_tFrame.dwSpeed = 55;
		Add_Stat_hp(-10);
		m_tStat.m_fSpeed = 2.5f;
		m_CoolDown = GetTickCount64();
	}

}

void CMarine::AttackToEnemy(CObj* _Enemey)
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

		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CMarineHit>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
		_Enemey->Add_Stat_hp(-Damge);

		m_AttackTime = GetTickCount64();
	}
}

void CMarine::StreamPackCoolDown()
{
	if (!m_bIsSteamPack) return;
	
	if (m_CoolDown + 3000 < GetTickCount64())
	{
		//CSoundMgr::Get_Instance()->PlaySFX();
		m_tFrame.dwSpeed = 100;
		m_bIsSteamPack = false;
		m_tStat.m_fSpeed = 1.8f;
	}

}

