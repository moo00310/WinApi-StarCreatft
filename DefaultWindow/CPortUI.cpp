#include "pch.h"
#include "CPortUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CPortUI::CPortUI() : m_pUintlist(nullptr), m_bRender(false), m_ePreState(PT_END), m_eCurState(PT_END)
{
}

CPortUI::~CPortUI()
{
}

void CPortUI::Initialize()
{
	Initailize_Img();
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
}

int CPortUI::Update()
{
	if (m_pUintlist->empty())
	{
		m_eCurState = PT_END;
		m_bRender = false;
		return 0;
	}

	if (m_pUintlist->size() == 1)
	{
		m_eCurState = PT_MARINE;
		m_bRender = true;
		Change_Port();
	}

	return 0;
}

void CPortUI::Late_Update()
{
	__super::Move_Frame();
}

void CPortUI::Render(HDC hdc)
{
	if (!m_bRender) return;
	
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hdc,
	526, 520, 60, 56,
	hMemDC,
	60 * m_tFrame.iCurCount,
 	0,
 	SRCCOPY);

	//Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CPortUI::Release()
{
}

void CPortUI::Initailize_Img()
{
	// ¸¶¸°
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Marine/Marine.bmp", L"Marine_port");
	
	//SCV
}

void CPortUI::Change_Port()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case PT_COMMEND:
			break;
		case PT_SCV:
			m_pImgKey = L"SCVPort";

			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case PT_MARINE:
			m_pImgKey = L"Marine_port";
			Anime_Port(0, 44);
			break;


		case PT_END:
			m_pImgKey = L"";
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPortUI::Anime_Port(int Start, int End)
{
	m_tFrame.iFrameStart = Start;
	m_tFrame.iFrameEnd = End;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount64();
}