#include "pch.h"
#include "CWireUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

#include "CKeyMgr.h"


CWireUI::CWireUI() :m_eID(OT_Unit_End), MaxHp(0.f), NowHp(0.f), UnitID(0), Text_X(0)
{
}

CWireUI::~CWireUI()
{
}

void CWireUI::Initialize()
{
	m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
	m_tInfo = { 260, 530 ,64, 64 };
}

int CWireUI::Update()
{
	if (m_pUintlist->size() == 1)
	{
		m_eID = m_pUintlist->front()->Get_ObjID();
		m_bRender = true;
		m_pImgKey = L"BigWire";
		Change_Wire();
	}
	else
	{
		m_bRender = false;
		m_eID = OT_END;
		m_pImgKey = L"";
	}

	__super::Update_Rect();
    return 0;
}

void CWireUI::Late_Update()
{
}

void CWireUI::Render(HDC hdc)
{
	if (!m_bRender) return;

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);


	GdiTransparentBlt(hdc,			// 복사 받을 DC
		m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top,
		64,			// 복사 받을 이미지의 가로, 세로
		64,
		hMemDC,						// 복사할 이미지 DC	
		64 * m_tFrame.iCurCount,
		64 * UnitID,
		64,										// 복사할 이미지의 가로, 세로
		64,
		RGB(0, 0, 0));		// 제거할 색상

	TextPrint(hdc);
	TextName(hdc);
}

void CWireUI::Release()
{
}

void CWireUI::Change_Wire()
{
	MaxHp = m_pUintlist->front()->Get_Stat()->m_iMaxHp;
	NowHp = m_pUintlist->front()->Get_Stat()->m_iHp;
	float damage = MaxHp / 6.f;

	int frameCount = static_cast<int>((MaxHp - NowHp) / damage);
	m_tFrame.iCurCount = min(frameCount, 5); // 최대값 5로 제한
}

void CWireUI::TextPrint(HDC hdc)
{
	wchar_t	m_wcHp[32] = L"";

	// 텍스트 색상과 배경 설정
	SetTextColor(hdc, RGB(0, 200, 0));     // 텍스트 색상
	//SetBkColor(hdc, RGB(0, 255, 0));      // 배경 색상
	SetBkMode(hdc, TRANSPARENT);              // 배경 모드: 투명

	// 텍스트 출력
	swprintf_s(m_wcHp, 32, L"%d / %d", (int)MaxHp, (int)NowHp);
	TextOut(hdc, Text_X, 560, m_wcHp, (int)wcslen(m_wcHp));
}

void CWireUI::TextName(HDC hdc)
{
	wchar_t	m_wcHp[32] = L"";
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	switch (m_eID)
	{
	case OT_Scv:
		swprintf_s(m_wcHp, 32, L"테란 SCV");
		Text_X = 240;
		UnitID = 0;
		break;
	case OT_Marine:
		swprintf_s(m_wcHp, 32, L"테란 마린");
		Text_X = 240;
		UnitID = 1;
		break;
	case OT_Medic:
		swprintf_s(m_wcHp, 32, L"테란 메딕");
		Text_X = 240;
		UnitID = 4;
		break;
	case OT_Ghost:
		swprintf_s(m_wcHp, 32, L"테란 고스트");
		Text_X = 240;
		UnitID = 3;
		break;
	case OT_Tank:
		swprintf_s(m_wcHp, 32, L"테란 시스 탱크");
		Text_X = 240;
		UnitID = 6;
		break;
	case OT_SiegeTank:
		swprintf_s(m_wcHp, 32, L"테란 시스 탱크");
		Text_X = 240;
		UnitID = 7;
		break;
	case OT_Battlecruiser:
		swprintf_s(m_wcHp, 32, L"테란 배틀크루져");
		Text_X = 240;
		UnitID = 11;
		break;
	case OT_Unit_End:
		break;
	case OT_Commend:
		swprintf_s(m_wcHp, 32, L"테란 커맨드 센터");
		Text_X = 220;
		UnitID = 15;
		break;
	case OT_Suffly:
		swprintf_s(m_wcHp, 32, L"테란 보급고");
		Text_X = 220;
		UnitID = 16;
		break;
	case OT_Refinery:
		swprintf_s(m_wcHp, 32, L"테란 정제소");
		Text_X = 220;
		UnitID = 17;
		break;
	case OT_Barrck:
		swprintf_s(m_wcHp, 32, L"테란 병영");
		Text_X = 220;
		UnitID = 18;
		break;
	case OT_Academy:
		swprintf_s(m_wcHp, 32, L"테란 아카데미");
		Text_X = 220;
		UnitID = 21;
		break;
	case OT_Factory:
		swprintf_s(m_wcHp, 32, L"테란 군수공장");
		Text_X = 220;
		UnitID = 23;
		break;
	case OT_Addon:
		swprintf_s(m_wcHp, 32, L"테란 기계실");
		Text_X = 220;
		UnitID = 32;
		break;
	case OT_Armory:
		swprintf_s(m_wcHp, 32, L"테란 무기고");
		Text_X = 220;
		UnitID = 26;
		break;
	case OT_Starport:
		swprintf_s(m_wcHp, 32, L"테란 우주공항");
		Text_X = 220;
		UnitID = 24;
		break;
	case OT_StarportAddOn:
		swprintf_s(m_wcHp, 32, L"테란 관제탑");
		Text_X = 220;
		UnitID = 31;
		break;
	case OT_ScienceFacility:
		swprintf_s(m_wcHp, 32, L"테란 과학시설");
		Text_X = 220;
		UnitID = 25;
		break;
	case OT_ScienceSecret:
		swprintf_s(m_wcHp, 32, L"테란 비밀작전실");
		Text_X = 210;
		UnitID = 29;
		break;
	case OT_PhysicsLab:
		swprintf_s(m_wcHp, 32, L"테란 물리연구실");
		Text_X = 210;
		UnitID = 30;
		break;
	case OT_CmdNuke:
		swprintf_s(m_wcHp, 32, L"테란 핵격납고");
		Text_X = 220;
		UnitID = 28;
		break;
	case OT_Build_End:
		break;
	default:
		break;
	}

	TextOut(hdc, 360, 500, m_wcHp, (int)wcslen(m_wcHp));
}
