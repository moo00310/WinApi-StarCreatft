#include "pch.h"
#include "CMainUI.h"
#include "CBmpMgr.h"

void CMainUI::Initialize()
{

}

int CMainUI::Update()
{
	return 0;
}

void CMainUI::Late_Update()
{
}

void CMainUI::Render(HDC hdc)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MainUI");

	GdiTransparentBlt(hdc,	
		0,	
		0,
		800,		
		600,
		hMemDC,						
		0, 
		0,
		800,								
		600,
		RGB(0, 255, 255));

	//Ellipse(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMainUI::Release()
{
	
}
