#include "pch.h"
#include "CButtonMouse.h"
#include "CBmpMgr.h"

void CButtonMouse::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Mouse/Cursor.bmp", L"Cursor");
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    m_pImgKey = L"Cursor";

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 4;
    m_tFrame.iCurCount = 0;
    m_indexY = 0;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();
    m_eRender = RENDER_UI;
}

int CButtonMouse::Update()
{
    m_tInfo.fX = (float)ptMouse.x;
    m_tInfo.fY = (float)ptMouse.y;

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    LockMouse();
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CButtonMouse::Late_Update()
{
    ShowCursor(FALSE);
    CMouse::Move_Frame();
}

void CButtonMouse::Render(HDC hDC)
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
}

void CButtonMouse::Release()
{
}
