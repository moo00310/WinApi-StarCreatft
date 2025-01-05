#include "pch.h"
#include "E_Factory.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"

void E_Factory::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();
    m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 160.f;

    m_pImgKey = L"Factory_Blue";
    m_iTemplateSize = TS_NORMAL;
    m_bTemplate = false;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_Factory;
    m_tStat = { 1250.f, 1250.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Factory));

    m_bIsEnemy = true;

    __super::Update_Rect();
    Block_Map();
}

int E_Factory::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        // 터지는이펙트 & 사운드

        UnBlock_Map(); // 바닥 이동 불가 해제
        return OBJ_DEAD;
    }

    //KeyInput();
    //Spawn_Uint_CoolDown();     // 쿨타임 적용해서 유닛생성

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void E_Factory::Late_Update()
{
    Change_Motion();
    CBuild::Move_Frame();
}

void E_Factory::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_7_E");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX,
            m_tRect.top + iScrollY + 30,
            128,
            128,
            hFxDC,
            0,
            0,
            128,
            128,
            RGB(255, 0, 255));
    }

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

void E_Factory::Release()
{
}

void E_Factory::KeyInput()
{
    return;
}

void E_Factory::Change_Motion()
{
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
            m_tFrame.iFrameStart = 2;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.iCurCount = 2;
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
