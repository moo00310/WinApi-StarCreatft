#include "pch.h"
#include "CPortUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CPortUI::CPortUI() : m_pUintlist(nullptr), m_eID(UNIT_END), m_bRender(false)
{
}

CPortUI::~CPortUI()
{
}

void CPortUI::Initialize()
{
	Initailize_Img();
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
	m_tInfo = { 555, 550 ,60, 56 };
}

int CPortUI::Update()
{
	if (m_pUintlist->empty())
	{
		m_eID = UNIT_END;
		m_pImgKey = L"";
		m_bRender = false;
	}

	if (m_pUintlist->size() == 1)
	{
		m_eID = m_pUintlist->front()->Get_UnitID();
		m_bRender = true;
	}

	Change_Port();

	__super::Update_Rect();
	return 0;
}

void CPortUI::Late_Update()
{
	__super::Move_Frame();
}

void CPortUI::Render(HDC hdc)
{
	if (!m_bRender) return;
	
	//HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MainUI");

	/*BitBlt(hdc,
	0, 0, WINCX, WINCY,
	hMemDC,
 	0,
 	0,
 	SRCCOPY);*/

	Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CPortUI::Release()
{
}

void CPortUI::Initailize_Img()
{
	// ¸¶¸°
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/temp.bmp", L"MainUI");
	
	//SCV
}

void CPortUI::Change_Port()
{
	switch (m_eID)
	{
	case UNIT_END:
		break;
	case UNIT_SCV:
		m_pImgKey = L"marinePort";
		Anime_Port(0,14);
		break;
	case UNIT_MARINE:
		m_pImgKey = L"SCVPort";

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iCurCount = 0;
		m_tFrame.dwSpeed = 200;
		m_tFrame.dwTime = GetTickCount64();

		break;
	default:
		break;
	}
}

void CPortUI::Anime_Port(int Start, int End)
{
	m_tFrame.iFrameStart = Start;
	m_tFrame.iFrameEnd = End;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();
}