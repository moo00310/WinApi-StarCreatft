#include "pch.h"
#include "E_Scv.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CMapMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"

void E_Scv::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

	m_pImgKey = L"Scv_E";
	m_eObjID = OT_Scv;
	m_tStat = { 60.f, 60.f, 5, 0, 32, 2.3f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 2;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

	A_GroundPos.x = (int)m_tInfo.fX / 32;
	A_GroundPos.y = (int)m_tInfo.fY / 32;
}

int E_Scv::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"SCVDead.mp3", 0.8f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CSCVDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));

		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void E_Scv::Late_Update()
{
}

void E_Scv::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_1_E");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 17,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 20,
			40,			// 복사 받을 이미지의 가로, 세로
			40,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			40,										// 복사할 이미지의 가로, 세로
			40,
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

void E_Scv::Release()
{
}

void E_Scv::Change_Motion()
{
}

void E_Scv::KeyInput()
{
}

void E_Scv::AttackToEnemy(CObj* _Enemey)
{
}
