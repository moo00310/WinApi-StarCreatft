#include "pch.h"
#include "CGameMouse.h"
#include "CScrollMgr.h"

/*---------------
    GameMouse
--------------------*/

CGameMouse::CGameMouse()
{
}

CGameMouse::~CGameMouse()
{
    Release();
}

void CGameMouse::Initialize()
{
    m_tInfo.fCX = 32.f;
    m_tInfo.fCY = 32.f;

    m_eRender = RENDER_UI;

}

int CGameMouse::Update()
{
    POINT       ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    SetScroll();
    ScrollMove(ptMouse);

    m_tInfo.fX = (float)ptMouse.x;
    m_tInfo.fY = (float)ptMouse.y;

    __super::Update_Rect();

    LockMouse();
    ShowCursor(FALSE);

    return OBJ_NOEVENT;
}

void CGameMouse::Late_Update()
{
}

void CGameMouse::Render(HDC hDC)
{
    //HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CGameMouse::Release()
{
}

void CGameMouse::MouseInput()
{


}

void CGameMouse::ScrollMove(POINT mouse)
{
    if (mouse.x >= CScrollMgr::Get_Instance()->Get_ScrollX() + WINCX - 3)
        CScrollMgr::Get_Instance()->Set_ScrollX(-3.f);
    if (mouse.x <= CScrollMgr::Get_Instance()->Get_ScrollX() * -1 + 3)
        CScrollMgr::Get_Instance()->Set_ScrollX(3.f);

    if (mouse.y >= CScrollMgr::Get_Instance()->Get_ScrollY() + WINCY - 3)
        CScrollMgr::Get_Instance()->Set_ScrollY(-3.f);
    if (mouse.y <= CScrollMgr::Get_Instance()->Get_ScrollY() * -1 + 3)
        CScrollMgr::Get_Instance()->Set_ScrollY(3.f);
}
