#include "pch.h"
#include "CPortUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CPortUI::CPortUI()
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
		m_eCurState = OT_END;
		m_bRender = false;
		return 0;
	}
	if (m_pUintlist->size() >= 1)
	{
		m_eCurState = m_pUintlist->front()->Get_ObjID();
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
	// SCV
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/SCV/SCV.bmp", L"Scv_port");

	// 마린
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Marine/Marine.bmp", L"Marine_port");
	
	// 메딕
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Medic/Medic.bmp", L"Medic_port");

	// 고스트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Ghost/Ghost.bmp", L"Ghost_port");

	// 어드 바이저
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Advisor/Advosr.bmp", L"Advosr");
}

void CPortUI::Change_Port()
{
	if (m_ePreState != m_eCurState)
	{

		switch (m_eCurState)
		{
		case OT_Scv:
			m_pImgKey = L"Scv_port";
			Anime_Port(0, 44);
			break;
		case OT_Marine:
			m_pImgKey = L"Marine_port";
			Anime_Port(0, 44);
			break;
		case OT_Medic:
			m_pImgKey = L"Medic_port";
			Anime_Port(0, 44);
			break;
		case OT_Ghost:
			m_pImgKey = L"Ghost_port";
			Anime_Port(0, 44);
			break;
		case OT_Tank:
			break;
		case OT_Science_Vessel:
			break;
		case OT_Commend:
		case OT_Suffly:
		case OT_Refinery:
		case OT_Barrck:
		case OT_Academy:
		case OT_Factory:
		case OT_Addon:
		case OT_Armory:
		case OT_Starport:
		case OT_StarportAddOn:
		case OT_ScienceSecret:
		case OT_ScienceFacility:
		case OT_CmdNuke:
			// 어드바이저 이미지
			m_pImgKey = L"Advosr";
			break;
		case OT_Unit_End:
		case OT_Build_End:
		case OT_END:
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