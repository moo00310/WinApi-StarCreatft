#include "pch.h"
#include "Player.h"
#include "AbstractFactory.h"
Player::Player() : m_pBullet(nullptr), m_dwTime(GetTickCount64())
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

int Player::Update()
{
	// 키 입력 값 받는 곳
	Key_Input();

	// Player 위치 조정
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void Player::Late_Update()
{
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

	if (m_dwTime + 150 < GetTickCount64())
	{
		if (GetAsyncKeyState('W')) m_pBullet->push_back(Create_Bullet(DIR_UP));
		if (GetAsyncKeyState('A')) m_pBullet->push_back(Create_Bullet(DIR_LEFT));
		if (GetAsyncKeyState('S')) m_pBullet->push_back(Create_Bullet(DIR_DOWN));
		if (GetAsyncKeyState('D'))
		{
			m_pBullet->push_back(AbstractFactory<Bullet>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));
		}
	}
}

Obj* Player::Create_Bullet(DIRECTION eDir)
{
	Obj* pBullet = AbstractFactory<Bullet>::Create();
	pBullet->Set_Pos(m_tInfo.fX, m_tInfo.fY);
	pBullet->SetDir(eDir);

	return pBullet;
}