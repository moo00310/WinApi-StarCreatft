#include "pch.h"
#include "CWireUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

#include "CKeyMgr.h"


void CWireUI::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Wire/BigWire.bmp", L"BigWire");
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
	m_tInfo = { 270, 530 ,64, 64 };
}

int CWireUI::Update()
{
	if (m_pUintlist->size() == 1)
	{
		m_eID = m_pUintlist->front()->Get_UnitID();
		m_bRender = true;
		m_pImgKey = L"BigWire";
		Change_Port();
	}
	else
	{
		m_bRender = false;
		m_eID = UNIT_END;
		m_pImgKey = L"";
	}

	__super::Update_Rect();
    return 0;
}

void CWireUI::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_pUintlist->front()->Add_Stat_hp(-6);
	}
}

void CWireUI::Render(HDC hdc)
{
	if (!m_bRender) return;

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	BitBlt(hdc,
	m_tRect.left,m_tRect.top, 64, 64,
	hMemDC,
	64 * m_tFrame.iCurCount,
	64 * (int)m_eID,
	SRCCOPY);

	//Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CWireUI::Release()
{
}

void CWireUI::Change_Port()
{
	int MaxHp = m_pUintlist->front()->Get_Stat()->m_iMaxHp;
	int NowHp = m_pUintlist->front()->Get_Stat()->m_iHp;
	float damage = MaxHp / 6;

	int frameCount = static_cast<int>((MaxHp - NowHp) / damage);
	m_tFrame.iCurCount = min(frameCount, 5); // 최대값 5로 제한
}
