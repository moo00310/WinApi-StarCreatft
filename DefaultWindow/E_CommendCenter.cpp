#include "pch.h"
#include "E_CommendCenter.h"
#include "CObjMgr.h"
#include "CMapMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CSoundMgr.h"

void E_CommendCenter::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();
    m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

    m_tInfo.fCX = 128.f;
    m_tInfo.fCY = 160.f;

    m_pImgKey = L"CommandCenter_Blue";
    m_bTemplate = false;
    m_eCurState_Build = BS_IDLE;
    m_eObjID = OT_Commend;
    m_tStat = { 1500.f, 750.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;

    m_bIsEnemy = true;

    __super::Update_Rect();
    Block_Map();
}

int E_CommendCenter::Update()
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

void E_CommendCenter::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    CBuild::Move_Frame();
}

void E_CommendCenter::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    // 선택
    HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_8_E");
    if (m_bSelect)
    {
        GdiTransparentBlt(hDC,			// 복사 받을 DC
            m_tRect.left + iScrollX - 10,	// 복사 받을 위치 좌표 X, Y	
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

void E_CommendCenter::Release()
{
}

void E_CommendCenter::KeyInput()
{
    return;
}

void E_CommendCenter::Change_Motion()
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
