#include "pch.h"
#include "MainGame.h"
#include "AbstractFactory.h"
#include "CollisionMgr.h"

MainGame::MainGame() : m_dwTime(GetTickCount64()), m_iFPS(0), m_hDC(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(TCHAR));
}

MainGame::~MainGame()
{
	Release();
}

void MainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	m_ObjList[OBJ_PLAYER].push_back(AbstractFactory<Player>::Create());
	// 플레이어의 리스트의 주소를 불렛에 전달
	dynamic_cast<Player*>(m_ObjList[OBJ_PLAYER].front())->SetBulletList(&m_ObjList[OBJ_BULLET]);

	for (int i = 0; i < 3; ++i)
	{
		m_ObjList[OBJ_MONSTER].push_back(AbstractFactory<Monster>::Create(200.f, (i + 1) * 150.f, DIR_END));
	}
}

void MainGame::Update()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<Obj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
	
}

void MainGame::Late_Update()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Late_Update();
		}
	}

	// 충돌 처리
	//CollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
	CollisionMgr::Collision_Circle(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
}


void MainGame::Render()
{
#pragma region FPS 출력

	m_iFPS++;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

#pragma endregion

	// 먼저 그려서 배경을 만듦 (잔상가리기 용)
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
	Rectangle(m_hDC, 100, 100, WINCX - 100, WINCY - 100);

	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(m_hDC);
	}
}

void MainGame::Release()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<Obj*>);
		m_ObjList[i].clear();
	}

	// RefCount를 이용한 메모리 반환
	ReleaseDC(g_hWnd, m_hDC);
}
