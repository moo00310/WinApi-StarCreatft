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

void CMouse::Move_Frame()
{
    if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
    {
        ++m_tFrame.iCurCount;

        if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
            m_tFrame.iCurCount = m_tFrame.iFrameStart;

        m_tFrame.dwTime = GetTickCount64();
    }
}

/*---------------
    EditMouse
--------------------*/

EditMouse::EditMouse() : m_iOption(0) , m_eEditType(ET_END), _iX(0)
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

    m_pImgKey = L"Tilesquare";
}

int EditMouse::Update()
{
    POINT       ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);
    SetScroll();
  
    m_tInfo.fX = (float)ptMouse.x;
    m_tInfo.fY = (float)ptMouse.y;

    ChangeMouse();
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

    GdiTransparentBlt(hDC,			// 복사 받을 DC
        (int)(m_tRect.left/32) * 32,	// 복사 받을 위치 좌표 X, Y	
        (int)(m_tRect.top/32) * 32,
        (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        hMemDC,						// 복사할 이미지 DC	
        _iX * m_iOption, // 비트맵 출력 시작 좌표(Left, top)
        0,
        (int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
        (int)m_tInfo.fCY,
        RGB(0, 255, 0));		// 제거할 색상

}

void EditMouse::Release()
{

}

void EditMouse::ChangeMouse()
{
    if (m_eEditType == ET_TILE)
    {
        _iX = 32;
        m_pImgKey = L"Tilesquare";
        m_tInfo.fCX = 32.f;
        m_tInfo.fCY = 32.f;
    }

    if (m_eEditType == ET_Resourece)
    {
        _iX = 64;
        m_pImgKey = L"Resource";
        if (m_iOption >= 2)
        {
            m_tInfo.fCX = 128.f;
            m_tInfo.fCY = 64.f;
        }
        else
        {
            m_tInfo.fCX = 64.f;
            m_tInfo.fCY = 64.f;
        }
    }

}


