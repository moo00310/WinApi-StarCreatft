#include "pch.h"
#include "CEnding.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

void CEnding::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Win/Ending.bmp", L"Ending");
	CSoundMgr::Get_Instance()->PlayBGM(L"WinBgm.mp3",0.8f);
}

int CEnding::Update()
{
	return 0;
}

void CEnding::Late_Update()
{
}

void CEnding::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Ending");

	BitBlt(hDC,
		0, 0, 800, 600,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CEnding::Release()
{
}
