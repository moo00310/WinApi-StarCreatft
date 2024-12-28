#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CMouseMgr.h"

#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CTileMgr.h"
#include "CMapMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CMemoryPoolMgr.h"
#include "CBloodEffect.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount64()), m_iFPS(0), m_hDC(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	CSoundMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CSceneMgr::Get_Instance()->Set_Scene(SC_LOGO);
	CMemoryPoolMgr::Get_Instance(300, 100);


#pragma region 콘솔 디버그
	//#ifdef _DEBUG
	//
	//	if (::AllocConsole() == TRUE)
	//	{
	//		FILE* nfp[3];
	//		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
	//		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
	//		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
	//		std::ios::sync_with_stdio();
	//	}
	//
	//#endif // _DEBUG
	//
//#ifdef _DEBUG
//
//		if (m_dwTime + 2000 <= GetTickCount64())
//		{
//			Pos pos = { (int)ptMouse.y / 32 , (int)ptMouse.x / 32 };
//			cout << "-------------------------------------------------" << endl;
//
//			cout << CMapMgr::Get_Instance()->GetTileType(pos) << endl;
//
//
//			cout << "-------------------------------------------------" << endl;
//			m_dwTime = GetTickCount64();
//		}
//
//#endif // DEBUG


#pragma endregion

}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}


void CMainGame::Render()
{
#pragma region  FPS 출력
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}
#pragma endregion
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hMemDC);

	BitBlt(m_hDC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);

}

void CMainGame::Release()
{
	CTileMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::DestroyInstance();
	CMapMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CMouseMgr::DestroyInstance();
	CSoundMgr::Destroy_Instance();
	CMemoryPoolMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
