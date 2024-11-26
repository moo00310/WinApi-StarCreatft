#include "pch.h"
#include "MainGame.h"

MainGame::MainGame() : m_hDC(nullptr), m_pPlayer(nullptr), m_pMonster(nullptr)
{
}

MainGame::~MainGame()
{
	Release();
}

void MainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	if (!m_pPlayer)
	{
		m_pPlayer = new Player();
		m_pPlayer->Initialize();
	}
	if(!m_pMonster)
	{
		m_pMonster = new Monster();
		m_pMonster->Initialize();
	}

	// 멤버 리스트 주소를 플레이어에 전달
	dynamic_cast<Player*>(m_pPlayer)->SetBulletList(&m_BulletList);
}

void MainGame::Update()
{
	
	m_pPlayer->Update();
	if (m_pMonster) { m_pMonster->Update(); }

	m_BulletList.remove_if([](Obj* bullet) {
		if (bullet->GetInfo().fX <= 100 || bullet->GetInfo().fY <= 100
			|| bullet->GetInfo().fX > WINCX - 100 || bullet->GetInfo().fY > WINCY - 100)
		{
			delete bullet;
			return true;
		}
		else
			return false;
		 });

	for (auto& pBullet : m_BulletList)
	{
		pBullet->Update();
	}

	
	for (auto& pBullet : m_BulletList)
	{
		RECT rc;
		if (m_pMonster == nullptr) return;
		if (IntersectRect(&rc, pBullet->GetRect(), m_pMonster->GetRect()) == TRUE)
		{
			Safe_Delete<Obj*>(m_pMonster);
		}
	}
	
}

void MainGame::Render()
{
	// 먼저 그려서 배경을 만듦 (잔상가리기 용)

	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
	Rectangle(m_hDC, 100, 100, WINCX - 100, WINCY - 100);

	m_pPlayer->Render(m_hDC);
	if(m_pMonster){ m_pMonster->Render(m_hDC); }

	for (auto& pBullet : m_BulletList)
		pBullet->Render(m_hDC);
	
}

void MainGame::Release()
{

	Safe_Delete<Obj*>(m_pPlayer);
	Safe_Delete<Obj*>(m_pMonster);

	for_each(m_BulletList.begin(), m_BulletList.end(), Safe_Delete<Obj*>);
	m_BulletList.clear();

	// RefCount를 이용한 메모리 반환
	ReleaseDC(g_hWnd, m_hDC);
	
}
