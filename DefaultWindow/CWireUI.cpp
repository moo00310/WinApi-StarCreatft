#include "pch.h"
#include "CWireUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

#include "CKeyMgr.h"


CWireUI::CWireUI() : m_pUintlist(nullptr), m_eID(OT_Unit_End), m_bRender(false), MaxHp(0.f), NowHp(0.f)
{
}

CWireUI::~CWireUI()
{
}

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
	if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		if (m_pUintlist->empty()) return;

		m_pUintlist->front()->Add_Stat_hp(-6);
	}
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
		64 * (int)m_eID,
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
	swprintf_s(m_wcHp, 32, L"%d / %d", (int)NowHp, (int)MaxHp);
	TextOut(hdc, 245, 560, m_wcHp, (int)wcslen(m_wcHp));
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
		break;
	case OT_Marine:
		swprintf_s(m_wcHp, 32, L"테란 마린");
		break;
	case OT_Medic:
		swprintf_s(m_wcHp, 32, L"테란 메딕");
		break;
	case OT_Ghost:
		swprintf_s(m_wcHp, 32, L"테란 고스트");
		break;
	case OT_Tank:
		swprintf_s(m_wcHp, 32, L"테란 시스 탱크");
		break;
	case OT_Science_Vessel:
		swprintf_s(m_wcHp, 32, L"테란 사이언스 베슬");
		break;
	case OT_Unit_End:
		break;
	case OT_Commend:
		swprintf_s(m_wcHp, 32, L"테란 커맨드 센터");
		break;
	case OT_Suffly:
		swprintf_s(m_wcHp, 32, L"테란 보급고");
		break;
	case OT_Barrck:
		swprintf_s(m_wcHp, 32, L"테란 배럭");
		break;
	case OT_Build_End:
		break;
	default:
		break;
	}

	TextOut(hdc, 360, 500, m_wcHp, (int)wcslen(m_wcHp));
}
