#include "pch.h"
#include "CMyButton.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	if (!lstrcmp(L"single", m_pImgKey))
	{
		m_tInfo.fCX = 320.f;
		m_tInfo.fCY = 95.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 34;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}
	else if (!lstrcmp(L"editor", m_pImgKey))
	{
		m_tInfo.fCX = 136.f;
		m_tInfo.fCY = 119.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 84;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}

	else if (!lstrcmp(L"exit", m_pImgKey))
	{
		m_tInfo.fCX = 184.f;
		m_tInfo.fCY = 128.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 49;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}
	

	m_eRender = RENDER_UI;
}

int CMyButton::Update()
{
	__super::Update_Rect();
	Move_Frame();
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (!lstrcmp(L"single", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_STAGE);

			else if (!lstrcmp(L"editor", m_pImgKey))
				CSceneMgr::Get_Instance()->Set_Scene(SC_EDIT);

			else if (!lstrcmp(L"exit", m_pImgKey))
				DestroyWindow(g_hWnd);

			return;
		}
	}
}

void CMyButton::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	
	GdiTransparentBlt(hDC,			
						m_tRect.left,	
						m_tRect.top,
						(int)m_tInfo.fCX,	
						(int)m_tInfo.fCY,
						hMemDC,				
						(int)m_tInfo.fCX * m_tFrame.iCurCount,
						0,
						(int)m_tInfo.fCX,	
						(int)m_tInfo.fCY,
						RGB(0, 0, 0));
}

void CMyButton::Release()
{
}

void CMyButton::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iCurCount;

		if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
			m_tFrame.iCurCount = m_tFrame.iFrameStart;

		m_tFrame.dwTime = GetTickCount64();
	}
}
