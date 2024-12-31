#include "pch.h"
#include "CResource.h"
#include "CBmpMgr.h"

CResource::CResource(): m_iOption(0)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

void CResource::Initailize()
{
	m_tInfo.fCX = 64;
	m_tInfo.fCY = 64;
}

void CResource::Update()
{

	Update_Rect();
}

void CResource::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX , // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY ,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 255, 0));
}

void CResource::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
