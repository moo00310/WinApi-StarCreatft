#include "pch.h"
#include "E_Barrack.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CSoundMgr.h"
#include "CGameMgr.h"

void E_Barrack::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();

    m_tInfo.fCX = 192.f;
    m_tInfo.fCY = 160.f;

    m_pImgKey = L"Barracks_Blue";
    m_iTemplateSize = TS_LAGE;
    m_bTemplate = false;
    m_eCurState_Build = BS_RUN;
    m_eObjID = OT_Barrck;
    m_tStat = { 1000.f, 1000.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_iMyBuildTIme = get<3>(ObjCost.at(OT_Barrck));
    m_bIsEnemy = true;

    __super::Update_Rect();
    Block_Map();
}

int E_Barrack::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        //사운드
        CSoundMgr::Get_Instance()->PlaySFX(L"BuildBoom.mp3", 0.1f);

       //이미지
        CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBuildDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBuildDead_Wreck_Big>::CreateFX(m_tInfo.fX, m_tInfo.fY));
        FireRemove();

        UnBlock_Map(); // 바닥 이동 불가 해제
        return OBJ_DEAD;
    }

    FireSpwan();

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void E_Barrack::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    CBuild::Move_Frame();
}

void E_Barrack::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    if (m_bSelect)
    {
        HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_8_E");
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

void E_Barrack::Release()
{
}

void E_Barrack::KeyInput()
{
    return;
}

void E_Barrack::Change_Motion()
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
            m_tFrame.iFrameEnd = 3;
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
