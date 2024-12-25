#include "pch.h"
#include "CAddon.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"

void CAddon::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/FactoryAddOn.bmp", L"FactoryAddOn");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/BuildTemplate.bmp", L"BuildTemplate");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_3(64.64).bmp", L"Select_3");

    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 128.f;

    m_pImgKey = L"BuildTemplate";
    m_iTemplateSize = TS_NORMAL;
    m_bTemplate = true;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_Addon;
    m_tStat = { 750.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Addon));

    __super::Update_Rect();
    Block_Map();
}

int CAddon::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        // 터지는이펙트 & 사운드

        UnBlock_Map(); // 바닥 이동 불가 해제
        return OBJ_DEAD;
    }

    KeyInput();
    Spawn_Uint_CoolDown();     // 쿨타임 적용해서 유닛생성

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CAddon::Late_Update()
{
    Change_Motion();

    if (m_eCurState_Build < BS_IDLE) return;
    __super::Move_Frame();
}

void CAddon::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_3");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX +30,
            m_tRect.top + iScrollY +45,
            64,
            64,
            hFxDC,
            0,
            0,
            64,
            64,
            RGB(255, 0, 255));
    }

    if (m_bTemplate)
    {
        int CX = (int)m_tInfo.fX - (int)(BuildTemplate_Size.x * 0.5f);
        int CY = (int)m_tInfo.fY - (int)(BuildTemplate_Size.y * 0.5f);

        GdiTransparentBlt(hDC,
            CX + iScrollX,
            CY + iScrollY,
            (int)BuildTemplate_Size.x,
            (int)BuildTemplate_Size.y,
            hMemDC,
            (int)BuildTemplate_Size.x * m_iTemplateSize,
            (int)BuildTemplate_Size.y * m_tFrame.iCurCount,
            (int)BuildTemplate_Size.x,
            (int)BuildTemplate_Size.y,
            RGB(0, 255, 0));
    }
    else
    {

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
            RGB(255, 0, 255));		// 제거할 색상
    }
}

void CAddon::Release()
{
}

void CAddon::KeyInput()
{
    if (!m_bSelect) return;

    if (m_eCurState_Build == BS_MAKE ||
        m_eCurState_Build == BS_TEMP ||
        m_eCurState_Build == BS_LINK) return;


    // 시스 업그레이드
    if (CKeyMgr::Get_Instance()->Key_Down('C'))
    {
    }
}

void CAddon::Change_Motion()
{
    if (m_eCurState_Build == BS_TEMP)
    {
        m_pImgKey = L"BuildTemplate";

        if (m_iBuildCount < (m_iMyBuildTIme / 5) * 1)
            m_tFrame.iCurCount = 0;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 2)
            m_tFrame.iCurCount = 1;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 3)
            m_tFrame.iCurCount = 2;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 4)
        {
            m_bTemplate = false;
            m_eCurState_Build = BS_MAKE;
        }

        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp / m_iMyBuildTIme);
    }


    if (m_eCurState_Build == BS_MAKE)
    {
        m_pImgKey = L"FactoryAddOn";
        m_tFrame.iFrameStart = 0;
        m_tFrame.iCurCount = 0;
        m_tFrame.iFrameEnd = 0;
        if (m_iMyBuildTIme < m_iBuildCount)
        {
            m_eCurState_Build = BS_LINK;
            m_iBuildCount = 0;
        }
          
        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp / m_iMyBuildTIme);
    }

    if (m_eCurState_Build == BS_LINK)
    {
        if (m_iBuildCount < 5)
            m_tFrame.iCurCount = 0;
        else if (m_iBuildCount < 20)
            m_tFrame.iCurCount = 1;
        else if (m_iBuildCount < 40)
            m_tFrame.iCurCount = 2;
        else if (m_iBuildCount < 60)
            m_tFrame.iCurCount = 3;
        else if (m_iBuildCount > 60)
            m_eCurState_Build = BS_IDLE;

        m_iBuildCount++;
    }


    //반복 애니메니션
    if (m_ePreState_Bulid != m_eCurState_Build)
    {
        switch (m_eCurState_Build)
        {
        case BS_IDLE:
            m_tFrame.iFrameStart = 4;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.iCurCount = 4;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case BS_RUN:
            m_tFrame.iFrameStart = 5;
            m_tFrame.iFrameEnd = 8;
            m_tFrame.iCurCount = 5;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case BS_END:
            break;
        default:
            break;
        }

        m_ePreState_Bulid = m_eCurState_Build;
    }
}
