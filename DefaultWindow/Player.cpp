#include "pch.h"
#include "Player.h"

Player::Player() : m_pBullet(nullptr)
{
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 100.f, 100.f, };
	m_fSpeed = 10.f;
}

void Player::Update()
{
	// 키 입력 값 받는 곳
	Key_Input();

	// Player 위치 조정
	__super::Update_Rect();
}

void Player::Render(HDC hDC)
{
	Rectangle(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void Player::Release()
{
}

void Player::Key_Input()
{
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		m_tInfo.fY -= m_fSpeed;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		m_tInfo.fY += m_fSpeed;
	}

	if (GetAsyncKeyState('W'))
	{
		m_pBullet->push_back(Create_Bullet(Bullet::W));
	}
	if (GetAsyncKeyState('A'))
	{
		m_pBullet->push_back(Create_Bullet(Bullet::A));
	}
	if (GetAsyncKeyState('S'))
	{
		m_pBullet->push_back(Create_Bullet(Bullet::S));
	}
	if (GetAsyncKeyState('D'))
	{
		m_pBullet->push_back(Create_Bullet(Bullet::D));
	}
}

Obj* Player::Create_Bullet(Bullet::WASD _wasd)
{
	Obj* pBullet = new Bullet(_wasd);

	pBullet->Initialize();
	pBullet->Set_Pos(m_tInfo.fX, m_tInfo.fY);

	return pBullet;
}
