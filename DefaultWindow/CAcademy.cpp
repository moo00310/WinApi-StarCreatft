#include "pch.h"
#include "CAcademy.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CGameMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CBloodEffect.h"

void CAcademy::Initialize()
{
    m_Map = CMapMgr::Get_Instance()->GetMap();

  m_tInfo.fCX = 96.f;
    m_tInfo.fCY = 128.f;

    m_pImgKey = L"BuildTemplate";
    m_iTemplateSize = TS_NORMAL;
    m_bTemplate = true;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_Academy;
    m_tStat = { 600.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Academy));

    __super::Update_Rect();
    Block_Map();
}

int CAcademy::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        CSoundMgr::Get_Instance()->PlaySFX(L"BuildBoom.mp3", 0.1f);

        //이미지
        CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBuildDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBuildDead_Wreck_Big>::CreateFX(m_tInfo.fX, m_tInfo.fY));

        UnBlock_Map(); // 바닥 이동 불가 해제
        return OBJ_DEAD;
    }

    KeyInput();
    Spawn_Uint_CoolDown();

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CAcademy::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    CBuild::Move_Frame();
}

void CAcademy::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_5");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX ,
            m_tRect.top + iScrollY + 25,
            96,
            96,
            hFxDC,
            0,
            0,
            96,
            96,
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
            RGB(0, 255, 0));		// 제거할 색상
    }
}

void CAcademy::Release()
{
}

void CAcademy::KeyInput()
{
    if (!m_bSelect) return;

    if (m_eCurState_Build == BS_MAKE ||
        m_eCurState_Build == BS_TEMP) return;


    // 마린 사거리업
    if (CKeyMgr::Get_Instance()->Key_Down('E'))
    {
        if (ChekList_OBJ(OT_Marine_SightUp)||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_SightUp)) return;
        
        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Marine_SightUp))
            m_listSpawn.push_back(OT_Marine_SightUp);
    }

    // 스팀팩 업그레이드
    if (CKeyMgr::Get_Instance()->Key_Down('T'))
    {
        if (ChekList_OBJ(OT_Marine_Streampack) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_Streampack)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Marine_Streampack))
            m_listSpawn.push_back(OT_Marine_Streampack);
    }

    // 메딕 마법 1
    if (CKeyMgr::Get_Instance()->Key_Down('Z'))
    {
        if (ChekList_OBJ(OT_Medic_Magic1) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Magic1)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Medic_Magic1))
            m_listSpawn.push_back(OT_Medic_Magic1);
    }
    // 메딕 마법 2
    if (CKeyMgr::Get_Instance()->Key_Down('X'))
    {
        if (ChekList_OBJ(OT_Medic_Magic2) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Magic2)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Medic_Magic2))
            m_listSpawn.push_back(OT_Medic_Magic2);
    }
    // 메딕 마나
    if (CKeyMgr::Get_Instance()->Key_Down('C'))
    {
        if (ChekList_OBJ(OT_Medic_Mana) ||
            CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Mana)) return;

        if (m_listSpawn.size() < 5 && CGameMgr::Get_Instance()->isBuying(OT_Medic_Mana))
            m_listSpawn.push_back(OT_Medic_Mana);
    }
}

void CAcademy::Change_Motion()
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
        m_pImgKey = L"Academy";
        m_tFrame.iFrameStart = 0;
        m_tFrame.iCurCount = 0;
        m_tFrame.iFrameEnd = 0;
        if (m_iMyBuildTIme < m_iBuildCount)
        {
            m_eCurState_Build = BS_IDLE;
            CGameMgr::Get_Instance()->AddTechCount(TECH_Academy, 1);
        }

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
