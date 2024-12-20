#include "pch.h"
#include "CIconUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CIconUI::CIconUI(): m_pUintlist(nullptr), m_bRender(false), m_ePreState(PT_END), m_eCurState(PT_END)
{
}

CIconUI::~CIconUI()
{
}

void CIconUI::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
}

int CIconUI::Update()
{
	if (m_pUintlist->empty())
	{
		m_eCurState = PT_END;
		m_bRender = false;
		return 0;
	}

	if (m_pUintlist->size() == 1)
	{
		// 해당 오브젝트의 아이디를 가져와야하고, 그 오브젝트의 상태도 가져 와야함 

		m_eCurState = PT_MARINE;
		m_bRender = true;
		Change_Port();
	}

}

void CIconUI::Late_Update()
{
}

void CIconUI::Render(HDC hdc)
{
	if (!m_bRender) return;

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hdc,
		526, 520, 60, 56,
		hMemDC,
		60 * m_tFrame.iCurCount,
		0,
		SRCCOPY);
}

void CIconUI::Release()
{
}

void CIconUI::Change_Port()
{
}

void CIconUI::Anime_Port(int Start, int End)
{
}
