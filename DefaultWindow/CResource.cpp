#include "pch.h"
#include "CResource.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CResource::CResource(): m_iOption(0)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

void CResource::Update()
{
	Update_Rect();
}

void CResource::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Resource");

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		64 * m_iOption, // 비트맵 출력 시작 좌표(Left, top)
		0 ,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));
}

void CResource::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CResource::SetOption(int num)
{
	m_iOption = num;

	if (num < 2)
	{
		m_tInfo.fCX = 64;
		m_tInfo.fCY = 64;
	}
	if (num == 2)
	{
		m_tInfo.fCX = 128;
		m_tInfo.fCY = 64;
	}
}
