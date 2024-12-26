#include "pch.h"
#include "CWireSmallUI.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"

CWireSmallUI::CWireSmallUI() : m_eID(OT_Unit_End), m_iUnitCount(0), MaxHp(0), NowHp(0), iCount(0)
{
}

CWireSmallUI::~CWireSmallUI()
{
}

void CWireSmallUI::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Wire/SmallWire.bmp", L"SmallWire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/UpGrade_Icon/upgradeDetail.bmp", L"upgradeDetail");
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
	m_tInfo = { 219, 510 ,32, 32 };
}

int CWireSmallUI::Update()
{
	if ((m_iUnitCount = m_pUintlist->size()) >= 2)
	{
		m_bRender = true;
		m_pImgKey = L"SmallWire";
	}
	else
	{
		m_bRender = false;
		m_pImgKey = L"";
	}

	__super::Update_Rect();
	return 0;
}

void CWireSmallUI::Late_Update()
{
}

void CWireSmallUI::Render(HDC hdc)
{
	if (!m_bRender) return;
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hMemDC2 = CBmpMgr::Get_Instance()->Find_Image(L"upgradeDetail");

	for_each(m_pUintlist->begin(), m_pUintlist->end(), [&](CObj* unit)
	{
		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left + 50 * ((int)iCount % 6),	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + 50 * ((int)iCount / 6),
			42,			// 복사 받을 이미지의 가로, 세로
			43,
			hMemDC2,						// 복사할 이미지 DC	
			0,
			0,
			42,										// 복사할 이미지의 가로, 세로
			43,
			RGB(255, 0, 255));


		Change_Wire(unit);
		m_eID = unit->Get_ObjID();

		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left+5 + 50 * ((int)iCount % 6),	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top+5 + 50 * ((int)iCount / 6),
			32,			// 복사 받을 이미지의 가로, 세로
			32,
			hMemDC,						// 복사할 이미지 DC	
			32 * (m_tFrame.iCurCount),
			32 * UnitWire(unit->Get_ObjID()),
			32,										// 복사할 이미지의 가로, 세로
			32,
			RGB(0, 0, 0));

		iCount++;
	});

	iCount = 0;

}

void CWireSmallUI::Release()
{
}

void CWireSmallUI::Change_Wire(CObj* unit)
{
	MaxHp = unit->Get_Stat()->m_iMaxHp;
	NowHp = unit->Get_Stat()->m_iHp;
	float damage = MaxHp / 6.f;

	int frameCount = static_cast<int>((MaxHp - NowHp) / damage);
	m_tFrame.iCurCount = min(frameCount, 5); // 최대값 5로 제한
}

int CWireSmallUI::UnitWire(OBJ_TYPE m_eID)
{
	switch (m_eID)
	{
	case OT_Scv:
		return 0;
		break;
	case OT_Marine:
		return 1;
		break;
	case OT_Medic:
		return 4;
		break;
	case OT_Ghost:
		return 3;
		break;
	case OT_Tank:
		return 6;
	case OT_SiegeTank:
		return 7;
		break;
	case OT_Science_Vessel:
		return 10;
		break;
	}
	return 999;
}