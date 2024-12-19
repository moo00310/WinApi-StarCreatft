#include "pch.h"
#include "CGameMouse.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CMarine.h"
#include "CCollisionMgr.h"

/*---------------
    GameMouse
--------------------*/

CGameMouse::CGameMouse() : m_eCurState(MS_IDLE), m_ePreState(MS_IDLE), m_indexY(0), m_UnitList(nullptr),
m_Select_UnitList(nullptr)
{
}

CGameMouse::~CGameMouse()
{
    Release();
}

void CGameMouse::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Mouse/Cursor.bmp", L"Cursor");

    m_pImgKey = L"Cursor";
    m_eRender = RENDER_UI;

    m_UnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
    m_Select_UnitList = CObjMgr::Get_Instance()->Get_Select_List();
}

int CGameMouse::Update()
{
    POINT       ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    ScrollMove(ptMouse);
    SetScroll();

    m_tInfo.fX = (float)ptMouse.x;
    m_tInfo.fY = (float)ptMouse.y;

    // 마우스 잠굼
    LockMouse();
   
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CGameMouse::Late_Update()
{
    POINT       ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    ColObject();
    Change_Cursor();
    MouseInput(ptMouse);
    ShowCursor(FALSE);
    __super::Move_Frame();
}

void CGameMouse::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

    GdiTransparentBlt(hDC,			// 복사 받을 DC
        m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
        m_tRect.top,
        (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        hMemDC,						// 복사할 이미지 DC	
        (int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
        (int)m_tInfo.fCY * m_indexY,
        (int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        RGB(255, 0, 255));

    //Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CGameMouse::Release()
{
}

void CGameMouse::MouseInput(POINT ptMouse)
{

    Pos temp = { int(ptMouse.y - CScrollMgr::Get_Instance()->Get_ScrollY()) / TILECY , int(ptMouse.x - CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECY };
    
    ///// 우클릭 : MOVE 
    if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
    {
        m_eCurState = MS_MOVE;
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CUnit* unit)
            {
                if (unit->Get_UnitID() != UNIT_END)
                {
                    unit->Astar(temp);
                    unit->SetInput(IP_MOVE);
                }
            });
    }
    if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
    {
        m_eCurState = MS_IDLE;
    }

    //// A - 좌클릭 : 어택 땅
    if (m_eCurState == MS_ATTACK && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
    {
        m_eCurState = MS_IDLE;

        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CUnit* unit)
            {
                if (unit != nullptr)
                {
                    unit->Astar(temp);
                    unit->SetInput(IP_ATTACK);
                }
            });
    }

    // 중간에 죽었을 때도 고민 해야함
    if (m_eCurState == MS_IDLE && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
    {
        // 유닛 선택 초기화
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CUnit* unit)
            {
                if (unit != nullptr)
                    unit->Set_Select(false);
            });
        m_Select_UnitList->clear();
    }

    if (CKeyMgr::Get_Instance()->Key_Down('A'))
    {
        m_eCurState = MS_ATTACK;
    }

    if (CKeyMgr::Get_Instance()->Key_Down('S'))
    {
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CUnit* unit)
            { 
                if (unit != nullptr)
                    unit->SetInput(IP_STOP);
            }); 
    }

    if (CKeyMgr::Get_Instance()->Key_Down('H'))
    {
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CUnit* unit)
            {
                if (unit != nullptr)
                    unit->SetInput(IP_HOLD);
            });
    }
}

void CGameMouse::ScrollMove(POINT mouse)
{
    if (mouse.x >=  WINCX - 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-3.f);
        m_eCurState = MS_SCROLL_R;
    }
    else if (m_eCurState == MS_SCROLL_R)
    {
        m_eCurState = MS_IDLE;
    }

    if (mouse.x <= 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(3.f);
        m_eCurState = MS_SCROLL_L;
    }
    else if (m_eCurState == MS_SCROLL_L)
    {
        m_eCurState = MS_IDLE;
    }

    if (mouse.y >= WINCY -20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(-3.f);
       m_eCurState = MS_SCROLL_D;
    }
    else if (m_eCurState == MS_SCROLL_D)
    {
        m_eCurState = MS_IDLE;
    }
    if (mouse.y <= 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(3.f);
        m_eCurState = MS_SCROLL_U;
    }
    else if (m_eCurState == MS_SCROLL_U)
    {
        m_eCurState = MS_IDLE;
    }
}

void CGameMouse::ColObject()
{
    CObj* unit(nullptr);
    // 전체 유닛 리스트에서 마우스랑 충돌했는지 검사
    if ((unit = CCollisionMgr::Collision_Rect_Mouse(m_tRect, *m_UnitList)) != nullptr)
    {
        m_eCurState = MS_OBJ;
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            m_Select_UnitList->clear();
            unit->Set_Select(true);
            CObjMgr::Get_Instance()->Add_SelectList(dynamic_cast<CUnit*>(unit));
        }
           
    }
    else if (MS_OBJ == m_eCurState)
    {
        m_eCurState = MS_IDLE;
    }
}

void CGameMouse::Change_Cursor()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case MS_IDLE:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.iCurCount = 0;
            m_indexY = 0;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_OBJ:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.iCurCount = 0;
            m_indexY = 2;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_ATTACK:
            m_tFrame.iFrameStart = 10;
            m_tFrame.iFrameEnd = 10;
            m_tFrame.iCurCount = 10;
            m_indexY = 5;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_MOVE:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.iCurCount = 0;
            m_indexY = 5;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_DRAG:
            m_tFrame.iFrameStart = 6;
            m_tFrame.iFrameEnd = 6;
            m_tFrame.iCurCount = 6;
            m_indexY = 0;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_R:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 1;
            m_tFrame.iCurCount = 0;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_UR:
            m_tFrame.iFrameStart = 2;
            m_tFrame.iFrameEnd = 3;
            m_tFrame.iCurCount = 2;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_U:
            m_tFrame.iFrameStart = 4;
            m_tFrame.iFrameEnd = 5;
            m_tFrame.iCurCount = 4;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_UL:
            m_tFrame.iFrameStart = 6;
            m_tFrame.iFrameEnd = 7;
            m_tFrame.iCurCount = 6;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_L:
            m_tFrame.iFrameStart = 8;
            m_tFrame.iFrameEnd = 9;
            m_tFrame.iCurCount = 8;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case MS_SCROLL_DL:
            m_tFrame.iFrameStart = 10;
            m_tFrame.iFrameEnd = 11;
            m_tFrame.iCurCount = 10;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_D:
            m_tFrame.iFrameStart = 12;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.iCurCount = 12;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_DR:
            m_tFrame.iFrameStart = 13;
            m_tFrame.iFrameEnd = 14;
            m_tFrame.iCurCount = 13;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        }

        m_ePreState = m_eCurState;
    }

}
