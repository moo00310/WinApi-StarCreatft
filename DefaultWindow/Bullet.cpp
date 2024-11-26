#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(WASD _wasd) : m_wasd(_wasd)
{

}

Bullet::~Bullet()
{
	Release();
}

void Bullet::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;
}

void Bullet::Update()
{
	switch (m_wasd)
	{
	case Bullet::W:
		m_tInfo.fY -= m_fSpeed;
		break;
	case Bullet::A:
		m_tInfo.fX -= m_fSpeed;
		break;
	case Bullet::S:
		m_tInfo.fY += m_fSpeed;
		break;
	case Bullet::D:
		m_tInfo.fX += m_fSpeed;
		break;
	default:
		break;
	}
	
	__super::Update_Rect();
}

void Bullet::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void Bullet::Release()
{
}
