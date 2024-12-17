#include "pch.h"
#include "CMouse.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

void CMouse::SetScroll()
{
    m_iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    m_iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
}

/*---------------
    EditMouse
--------------------*/

EditMouse::EditMouse() : m_iDrawID(0) , m_dwTime(GetTickCount64())
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


#ifdef  _DEBUG   

    if (m_dwTime + 1000 < GetTickCount64())
    {
        int		x = ptMouse.x / TILECX;
        int		y = ptMouse.y / TILECY;
        int	iIndex = y * TILEHIGHT + x;

        cout << "마우스 인덱스 : " << iIndex << flush;
        m_dwTime = GetTickCount64();
    }
#endif //  _DEBUG


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

   //Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
    
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
}

int GameMouse::Update()
{
    return 0;
}

void GameMouse::Late_Update()
{
}

void GameMouse::Render(HDC hDC)
{
}

void GameMouse::Release()
{
}

