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
    RECT rect = {};

    if (GetClientRect(g_hWnd, &rect))
    {
        POINT lt = { rect.left, rect.top };    // 왼쪽 위 좌표
        POINT rb = { rect.right, rect.bottom }; // 오른쪽 아래 좌표

        // 클라이언트 좌표를 화면 좌표로 변환
        ClientToScreen(g_hWnd, &lt);
        ClientToScreen(g_hWnd, &rb);

        // 화면 좌표로 영역 설정
        rect.left = lt.x;
        rect.top = lt.y;
        rect.right = rb.x;
        rect.bottom = rb.y;

        // 마우스 커서를 이 영역에 가둠
        ClipCursor(&rect);
    };

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
