#include "pch.h"
#include "CBloodEffect.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CMarineDead::CMarineDead() : m_iCount(0), m_iDeadImg(0)
{
}

CMarineDead::~CMarineDead()
{
	Release();
}

void CMarineDead::Initialize()
{
    m_pImgKey = L"Marine";
    
	m_tInfo.fX = 0;
	m_tInfo.fY = 0;
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

void CMarineDead::Late_Update()
{
	return;
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

void CMarineDead::Release()
{
}
