#include "pch.h"
#include "CMouse.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"

void CMouse::SetScroll()
{
    m_iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    m_iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
}

void CMouse::LockMouse()
{
}

/*---------------
    EditMouse
--------------------*/

EditMouse::EditMouse() : m_iDrawID(0) , m_dwTime(GetTickCount64()), m_eEditType(ET_END)
{
}

EditMouse::~EditMouse()
{
    Release();
}

void EditMouse::Initialize()
{
    m_tInfo.fCX = 32.f;
    m_tInfo.fCY = 32.f;

    m_pImgKey = L"Tile";
}

int EditMouse::Update()
{
    POINT       ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);
    SetScroll();
  
    m_tInfo.fX = (float)ptMouse.x - m_iScrollX;
    m_tInfo.fY = (float)ptMouse.y - m_iScrollY;

    LockMouse();

    
    __super::Update_Rect();

    ShowCursor(FALSE);


    return OBJ_NOEVENT;
}

void EditMouse::Late_Update()
{

}

void EditMouse::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    int iX(0), iY(0);

    iX = m_iDrawID % 14;
    iY = m_iDrawID / 14;

    BitBlt(hDC,
        m_tRect.left + m_iScrollX,
        m_tRect.top + m_iScrollY,
        TILECX,
        TILECY,
        hMemDC,
        TILECX * iX,
        TILECY * iY,
        SRCCOPY);
}

void EditMouse::Release()
{

}



/*---------------
    GameMouse
--------------------*/

GameMouse::GameMouse()
{
}

GameMouse::~GameMouse()
{
    Release();
}

void GameMouse::Initialize()
{
    m_tInfo.fCX = 32.f;
    m_tInfo.fCY = 32.f;

    m_eRender = RENDER_UI;
}

int GameMouse::Update()
{
    POINT       ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);
    SetScroll();
    ScrollMove(ptMouse);

    m_tInfo.fX = (float)ptMouse.x - m_iScrollX;
    m_tInfo.fY = (float)ptMouse.y - m_iScrollY;

    LockMouse();

    __super::Update_Rect();

    ShowCursor(FALSE);

    return OBJ_NOEVENT;
}

void GameMouse::Late_Update()
{
}

void GameMouse::Render(HDC hDC)
{
    //HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void GameMouse::Release()
{
}

void GameMouse::MouseInput()
{

    
}

void GameMouse::ScrollMove(POINT mouse)
{
    if(mouse.x >= CScrollMgr::Get_Instance()->Get_ScrollX() + WINCX - 20)
        CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
    if (mouse.x <= CScrollMgr::Get_Instance()->Get_ScrollX() + 20)
        CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

    if (mouse.y >= CScrollMgr::Get_Instance()->Get_ScrollY() + WINCY - 20)
        CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
    if (mouse.y <= CScrollMgr::Get_Instance()->Get_ScrollY() + 20)
        CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
}

