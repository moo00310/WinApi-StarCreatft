#include "pch.h"
#include "Monster.h"

Monster::Monster() : m_bRight(true)
{
}

Monster::~Monster()
{
	Release();
}

void Monster::Initialize()
{
	m_tInfo = { 130, 130, 50, 50 };
	m_fSpeed = 5;
}

void Monster::Update()
{
	if (m_bRight)
		m_tInfo.fX += m_fSpeed;
	else
		m_tInfo.fX -= m_fSpeed;


	if (m_tInfo.fX >= WINCX - 100)
	{
		m_bRight = false;
	}
	else if (m_tInfo.fX <= 100)
	{
		m_bRight = true;
	}
		
	__super::Update_Rect();
}

void Monster::Render(HDC hDC)
{
	Rectangle(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void Monster::Release()
{
}
