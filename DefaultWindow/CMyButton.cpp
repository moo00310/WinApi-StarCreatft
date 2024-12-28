#include "pch.h"
#include "CMyButton.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"

CMyButton::CMyButton() : m_iCount(0), m_Name(nullptr)
{
	ZeroMemory(&m_tButton, sizeof(INFO));
	ZeroMemory(&m_tButtonRect, sizeof(INFO));
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

		m_tButton = { 200.f, 150.f,143.f,22.f };
		m_Name = L"single_button";
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

		m_tButton = { 650.f, 150.f, 70.f, 25.f};
		m_Name = L"editor_button";
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

		m_tButton = { 550.f, 500.f, 49.f, 24.f};
		m_Name = L"exit_button";
	}
	

	m_eRender = RENDER_UI;
}

int CMyButton::Update()
{
	__super::Update_Rect();
	Update_Button_Rect();
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
		m_iCount = 1;
	}
	else
	{
		m_iCount = 0;
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

	HDC		hButtonDC = CBmpMgr::Get_Instance()->Find_Image(m_Name);

	GdiTransparentBlt(hDC,
		m_tButtonRect.left,
		m_tButtonRect.top,
		(int)m_tButton.fCX,
		(int)m_tButton.fCY,
		hButtonDC,
		(int)m_tButton.fCX * m_iCount,
		0,
		(int)m_tButton.fCX,
		(int)m_tButton.fCY,
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

void CMyButton::Update_Button_Rect()
{
	m_tButtonRect.left = LONG(m_tButton.fX - (m_tButton.fCX * 0.5f));
	m_tButtonRect.top = LONG(m_tButton.fY - (m_tButton.fCY * 0.5f));
	m_tButtonRect.right = LONG(m_tButton.fX + (m_tButton.fCX * 0.5f));
	m_tButtonRect.bottom = LONG(m_tButton.fY + (m_tButton.fCY * 0.5f));
}
