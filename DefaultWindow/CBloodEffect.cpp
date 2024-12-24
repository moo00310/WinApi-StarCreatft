#include "pch.h"
#include "CBloodEffect.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"


/*--------------------
  마린 사망 효과
----------------------*/

void CMarineDead::Initialize()
{
    m_pImgKey = L"Marine";
    
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 50.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CMarineDead::Update()
{
	m_iCount++;
	//테스트
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{ 
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount > 100)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}


void CMarineDead::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * 16 ,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 255, 0));		// 제거할 색상

	//Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}


/*--------------------
  메딕 사망 효과
----------------------*/

void CMedicDead::Initialize()
{

	m_pImgKey = L"Medic";

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CMedicDead::Update()
{
	m_iCount++;
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount > 100)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CMedicDead::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * 16,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));		// 제거할 색상

}


/*--------------------
  메딕 사망 효과
----------------------*/

void CGhostDead::Initialize()
{
	m_pImgKey = L"Ghost";

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CGhostDead::Update()
{
	m_iCount++;
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount > 100)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGhostDead::Render(HDC hDC)
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * 16,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상

}
