#include "pch.h"
#include "CRefinery.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CGameMgr.h"

void CRefinery::Initialize()
{
    m_Map = CMapMgr::Get_Instance()->GetMap();
    
    m_tInfo.fCX = 192.f;
    m_tInfo.fCY = 192.f;

    m_eObjID = OT_Refinery;
    m_tStat = { 750.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;
    m_eCurState_Build = BS_MAKE;
    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Refinery));
    //m_iMyBuildTIme = 80;

    __super::Update_Rect();
}

int CRefinery::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        // 터지는이펙트 & 사운드

        return OBJ_DEAD;
    }

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CRefinery::Late_Update()
{
    Change_Motion();
}

void CRefinery::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Refinery");

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_8");
        GdiTransparentBlt(hDC,			// 복사 받을 DC
            m_tRect.left + iScrollX + 20,	// 복사 받을 위치 좌표 X, Y	
            m_tRect.top + iScrollY + 20,
            148,			// 복사 받을 이미지의 가로, 세로
            148,
            hFxDC,						// 복사할 이미지 DC	
            0, // 비트맵 출력 시작 좌표(Left, top)
            0,
            148,										// 복사할 이미지의 가로, 세로
            148,
            RGB(255, 0, 255));		// 제거할 색상
    }

    GdiTransparentBlt(hDC,			// 복사 받을 DC
        m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
        m_tRect.top + iScrollY,
        (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        hMemDC,						// 복사할 이미지 DC	
        0,           // 비트맵 출력 시작 좌표(Left, top)
        (int)m_tInfo.fCY * m_tFrame.iCurCount,
        (int)m_tInfo.fCX,		// 복사할 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        RGB(0, 255, 0));		// 제거할 색상
}

void CRefinery::Release()
{
}

void CRefinery::KeyInput()
{
}

void CRefinery::Change_Motion()
{

    if (m_eCurState_Build == BS_MAKE)
    {
        if (m_iBuildCount < (m_iMyBuildTIme / 4) * 1)
            m_tFrame.iCurCount = 0;
        else if (m_iBuildCount < (m_iMyBuildTIme / 4) * 2)
            m_tFrame.iCurCount = 1;
        else if (m_iBuildCount < (m_iMyBuildTIme / 4) * 3)
            m_tFrame.iCurCount = 2;
        else if (m_iBuildCount < (m_iMyBuildTIme / 4) * 4)
            m_tFrame.iCurCount = 3;
        else if (m_iBuildCount > (m_iMyBuildTIme / 4) * 4)
        {
            m_tFrame.iCurCount = 4;
            m_eCurState_Build = BS_IDLE;
        }

        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp / m_iMyBuildTIme);
    }

}
