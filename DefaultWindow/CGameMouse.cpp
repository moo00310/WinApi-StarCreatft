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
m_Select_UnitList(nullptr), isDrag(false)
{
    ZeroMemory(&ptMouse, sizeof(POINT));
    ZeroMemory(&m_DragStart, sizeof(POINT));
    ZeroMemory(&m_DragEnd, sizeof(POINT));
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
    MouseInput(ptMouse);

    ColObject();
    Change_Cursor();

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

    if (!isDrag) return;
    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    HPEN oldPen = (HPEN)SelectObject(hDC, newPen);
    HPEN oldBrush = (HPEN)SelectObject(hDC, newBrush);

    Rectangle(hDC, (int)m_DragStart.x, (int)m_DragStart.y, (int)m_DragEnd.x, (int)m_DragEnd.y);


    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);

    DeleteObject(newBrush);
    DeleteObject(newPen);

}

void CGameMouse::Release()
{
}

void CGameMouse::ClearList()
{
    // 유닛 선택 초기화
    for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
        {
            if (unit != nullptr)
                unit->Set_Select(false);
        });
    m_Select_UnitList->clear();
}

void CGameMouse::ClearDrag()
{
    ZeroMemory(&m_DragStart, sizeof(POINT));
    ZeroMemory(&m_DragEnd, sizeof(POINT));
}

void CGameMouse::MouseInput(POINT ptMouse)
{
    Pos temp = { int(ptMouse.y - CScrollMgr::Get_Instance()->Get_ScrollY()) / TILECY , int(ptMouse.x - CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECY };
    
    ///// 우클릭 : MOVE 
    if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
    {
        m_eCurState = MS_MOVE;
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
            {
                if (unit != nullptr)
                {
                    if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                    {
                        pUnit->Astar(temp);
                        pUnit->SetInput(IP_MOVE);
                    }
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

        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
            {
                if (unit != nullptr)
                {
                    if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                    {
                        pUnit->Astar(temp);
                        pUnit->SetInput(IP_ATTACK);
                    }
                }
            });
    }

    // 땅 좌클릭
    if (m_eCurState == MS_IDLE && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
    {
        ClearList();
        m_DragStart.x = m_tInfo.fX;
        m_DragStart.y = m_tInfo.fY;
        isDrag = true;
    }
    if (isDrag  == true && CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
    {
        m_eCurState = MS_DRAG;
        m_DragEnd.x = m_tInfo.fX;
        m_DragEnd.y = m_tInfo.fY;
    }

    if (isDrag == true &&  CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
    {
        isDrag = false;
        ColDrag();
        ClearDrag();
        m_eCurState = MS_IDLE;
    }

    if (CKeyMgr::Get_Instance()->Key_Down('A'))
    {
        m_eCurState = MS_ATTACK;
    }

    if (CKeyMgr::Get_Instance()->Key_Down('S'))
    {
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
            { 
                if (unit != nullptr)
                {
                    if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                    {
                        pUnit->SetInput(IP_STOP);
                    }
                }
            }); 
    }

    if (CKeyMgr::Get_Instance()->Key_Down('H'))
    {
        for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
            {
                if (unit != nullptr)
                {
                    if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                    {
                        pUnit->SetInput(IP_HOLD);
                    }
                }
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
    if (isDrag) return;

    CObj* unit(nullptr);
    // 전체 유닛 리스트에서 마우스랑 충돌했는지 검사
    if ((unit = CCollisionMgr::Collision_Rect_Mouse(m_tRect, *m_UnitList)) != nullptr)
    {
        m_eCurState = MS_OBJ;
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            ClearList();
            unit->Set_Select(true);
            CObjMgr::Get_Instance()->Add_SelectList(unit);
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
            m_tFrame.iFrameStart = 5;
            m_tFrame.iFrameEnd = 5;
            m_tFrame.iCurCount = 5;
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

void CGameMouse::ColDrag()
{
    RECT rc = { m_DragStart.x, m_DragStart.y, m_DragEnd.x, m_DragEnd.y };
    CCollisionMgr::Collision_Rect_Mouse_RECT(rc,*m_UnitList, m_Select_UnitList);
}
