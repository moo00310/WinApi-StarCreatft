#include "pch.h"
#include "CCommedCenter.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CMapMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CcmdNuke.h"
#include "CGameMgr.h"


void CCommedCenter::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();
    m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 160.f;

    m_pImgKey = L"BuildTemplate";
    m_iTemplateSize = TS_LAGE;
    m_bTemplate = true;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_Commend;
    m_tStat = { 1500.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Commend));

    __super::Update_Rect();
    Block_Map();
}

int CCommedCenter::Update()
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

void CCommedCenter::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    CBuild::Move_Frame();
}

void CCommedCenter::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    // 선택
    HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_8");
    if (m_bSelect)
    {
        GdiTransparentBlt(hDC,			// 복사 받을 DC
            m_tRect.left + iScrollX -10,	// 복사 받을 위치 좌표 X, Y	
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
            RGB(0, 255, 0));		// 제거할 색상
    }
}

void CCommedCenter::Release()
{
}

void CCommedCenter::KeyInput()
{
    if (!m_bSelect) return;

    if (m_eCurState_Build == BS_MAKE ||
        m_eCurState_Build == BS_TEMP) return;

    // SCV
    if (CKeyMgr::Get_Instance()->Key_Down('S'))
    {
        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Scv))
            m_listSpawn.push_back(OT_Scv);
        
    }

    // 뉴클리어 건설
    if (CKeyMgr::Get_Instance()->Key_Down('N'))
    {
        if (m_bIsAddOn) return;

        if (CGameMgr::Get_Instance()->GetTechCount(TECH_CovertOps) > 0 && 
            CGameMgr::Get_Instance()->isBuying(OT_CmdNuke))
        {
            m_listSpawn.push_back(OT_CmdNuke);
            CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD, CAbstractFactory<CcmdNuke>::Create(m_tInfo.fX + 90, m_tInfo.fY + 20));
            m_bIsAddOn = true;
        }
    }

    
}

void CCommedCenter::Change_Motion()
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
        m_pImgKey = L"CommandCenter";
        m_tFrame.iFrameStart = 0;
        m_tFrame.iCurCount = 0;
        m_tFrame.iFrameEnd = 0;
        if (m_iMyBuildTIme < m_iBuildCount)
            m_eCurState_Build = BS_IDLE;

        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp / m_iMyBuildTIme);
    }

    if (m_ePreState_Bulid != m_eCurState_Build)
    {
        switch (m_eCurState_Build)
        {
        case BS_IDLE:
            if (m_pImgKey != L"CommandCenter")
            {
                m_pImgKey = L"CommandCenter";
                m_bTemplate = false;
                m_tStat.m_iHp = m_tStat.m_iMaxHp;
            }
            m_tFrame.iFrameStart = 1;
            m_tFrame.iFrameEnd = 1;
            m_tFrame.iCurCount = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case BS_RUN:
            m_tFrame.iFrameStart = 1;
            m_tFrame.iFrameEnd = 1;
            m_tFrame.iCurCount = 1;
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
