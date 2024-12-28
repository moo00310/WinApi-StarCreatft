#include "pch.h"
#include "CAnimeButton.h"
#include "CBmpMgr.h"

void CAnimeButton::Initialize()
{
	if (!lstrcmp(L"singleon", m_pImgKey))
	{
		m_tInfo.fCX = 245.f;
		m_tInfo.fCY = 114.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 59;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}
	else if (!lstrcmp(L"editoron", m_pImgKey))
	{
		m_tInfo.fCX = 187.f;
		m_tInfo.fCY = 116.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 19;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}

	else if (!lstrcmp(L"exiton", m_pImgKey))
	{
		m_tInfo.fCX = 201.f;
		m_tInfo.fCY = 125.f;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 29;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount64();
	}


	m_eRender = RENDER_UI;
}

int CAnimeButton::Update()
{
	__super::Update_Rect();
	Move_Frame();
	return OBJ_NOEVENT;
}

void CAnimeButton::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}

}

void CAnimeButton::Render(HDC hDC)
{
	if (!m_bRender) return;

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

void CAnimeButton::Release()
{
}

void CAnimeButton::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iCurCount;

		if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
			m_tFrame.iCurCount = m_tFrame.iFrameStart;

		m_tFrame.dwTime = GetTickCount64();
	}
}
