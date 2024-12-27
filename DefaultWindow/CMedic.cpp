#include "pch.h"
#include "CMedic.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"

CMedic::CMedic()
{
}

CMedic::~CMedic()
{
}

void CMedic::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Medic/Medic.bmp", L"Medic");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_0(32.32).bmp", L"Select_0");

	m_pImgKey = L"Medic";
	m_eObjID = OT_Medic;
	m_tStat = { 60.f, 60.f, 0, 1, 64, 1.8f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 14;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;
}

int CMedic::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CMedicDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		//CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		//CSoundMgr::Get_Instance()->PlaySound(L"Marine_Dead_1.mp3", SOUND_EFFECT, 0.5f, true);

		return OBJ_DEAD;
	}

	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CMedic::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void CMedic::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_0");

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

void CMedic::Release()
{
}

void CMedic::Change_Motion()
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

void CMedic::KeyInput()
{
}

void CMedic::AttackToEnemy(CObj* _Enemey)
{
	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() &&
		m_tFrame.iCurCount == m_iAttackFrame)
	{
		DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
		ATTACKID Attack_id = m_tStat.m_eAttackID;
		float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));

		//CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CMarineHit>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
		_Enemey->Add_Stat_hp(-Damge);

		m_AttackTime = GetTickCount64();
	}
}