#include "pch.h"
#include "CLogo.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/StartDisplay.bmp", L"StartDisplay");
}

int CLogo::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Set_Scene(SC_MENU);
		return 0;
	}


	return 0;
}

void CLogo::Late_Update()
{
}

void CLogo::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"StartDisplay");

	BitBlt(hDC,
		0, 0, WINCX, WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CLogo::Release()
{
}
