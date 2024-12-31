#include "pch.h"
#include "CScienceFacility.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CCovertOps.h"
#include "CGameMgr.h"

void CScienceFacility::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();
    m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);


    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 96.f;

    m_pImgKey = L"BuildTemplate";
    m_iTemplateSize = TS_NORMAL;
    m_bTemplate = true;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_ScienceFacility;
    m_tStat = { 850.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_ScienceFacility));

    __super::Update_Rect();
    Block_Map();
}

int CScienceFacility::Update()
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

void CScienceFacility::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    CBuild::Move_Frame();
}

void CScienceFacility::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_7");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX -5,
            m_tRect.top + iScrollY -5,
            128,
            128,
            hFxDC,
            0,
            0,
            128,
            128,
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

void CScienceFacility::Release()
{
}

void CScienceFacility::KeyInput()
{
    if (!m_bSelect) return;

    if (m_eCurState_Build == BS_MAKE ||
        m_eCurState_Build == BS_TEMP) return;

    if (CKeyMgr::Get_Instance()->Key_Down('E'))
    {
        if (ChekList_OBJ(OT_Vessle_Emp) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Emp)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Vessle_Emp))
            m_listSpawn.push_back(OT_Vessle_Emp);
    }
    if (CKeyMgr::Get_Instance()->Key_Down('I'))
    {
        if (ChekList_OBJ(OT_Vessle_Irradiate) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Irradiate)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Vessle_Irradiate))
            m_listSpawn.push_back(OT_Vessle_Irradiate);
    }
    if (CKeyMgr::Get_Instance()->Key_Down('T'))
    {
        if (ChekList_OBJ(OT_Vessle_Mana) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Mana)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Vessle_Mana))
            m_listSpawn.push_back(OT_Vessle_Mana);
    }

    // 비밀 뭐시기
    if (CKeyMgr::Get_Instance()->Key_Down('C'))
    {
        if (m_bIsAddOn) return;
        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_ScienceSecret))
            m_listSpawn.push_back(OT_ScienceSecret);

        m_bIsAddOn = true;
    }
}

void CScienceFacility::Change_Motion()
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
        m_pImgKey = L"ScienceFacility";
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
            m_tFrame.iFrameStart = 1;
            m_tFrame.iFrameEnd = 1;
            m_tFrame.iCurCount = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case BS_RUN:
            m_tFrame.iFrameStart = 1;
            m_tFrame.iFrameEnd = 2;
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
