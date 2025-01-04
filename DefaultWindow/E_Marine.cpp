#include "pch.h"
#include "E_Marine.h"
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
#include "CCollisionMgr.h"

void E_Marine::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD);

	m_pImgKey = L"MarineBlue";
	m_eObjID = OT_Marine;
	m_tStat = { 40.f, 40.f, 6, 0, 128, 1.8f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 14;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;


	// 해당 좌표로 공격
	
	A_GroundPos = { 10,10 };
	Astar(A_GroundPos);
	m_eInput = IP_ATTACK;

}

int E_Marine::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		// 파랑 마린 죽는거로 바꾸어야 go
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CMarineDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Marine_Dead_1.mp3", SOUND_EFFECT, 0.5f, true);

		return OBJ_DEAD;
	}

	Update_State();
	__super::Update_Rect();


	if (m_dwTime + 2000 <= GetTickCount64())
	{
		cout << "-------------------------------------------------" << endl;

		switch (m_eCurState)
		{
		case STATE_IDLE:
			cout << "IDLE" << endl;
			break;
		case STATE_MOVE:
			cout << "MOVE" << endl;
			break;
		case STATE_ATTACK:
			cout << "ATTACK" << endl;
			break;
		case STATE_SHOOT:
			cout << "ATTACK" << endl;
			break;
		case STATE_DEAD:
			break;
		case STATE_END:
			break;
		default:
			break;
		}

		cout << "-------------------------------------------------" << endl;
		m_dwTime = GetTickCount64();
	}


	return OBJ_NOEVENT;
}

void E_Marine::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void E_Marine::Render(HDC hDC)
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

void E_Marine::Release()
{
}

void E_Marine::Change_Motion()
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
			m_tFrame.dwSpeed = 100;
			break;


		case STATE_SHOOT:
			m_tFrame.iFrameStart = 13;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iCurCount = 13;
			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwSpeed = 100;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void E_Marine::KeyInput()
{
	return;
}

void E_Marine::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT) m_eCurState = STATE_SHOOT;
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

