#include "pch.h"
#include "CMiniMap.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

void CMiniMap::Initialize()
{
    CObjMgr* mgr = CObjMgr::Get_Instance();

    m_pPlayer = mgr->Get_ObjList(OBJ_PLAYER);
    m_pBuild = mgr->Get_ObjList(OBJ_BUILD);
    m_pEnemy = mgr->Get_ObjList(OBJ_MONSTER);
    m_pBuild_E = mgr->Get_ObjList(OBJ_BUILD_E);
}

int CMiniMap::Update()
{
	return 0;
}

void CMiniMap::Late_Update()
{
}

void CMiniMap::Render(HDC hdc)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MiniMap");

	// 1번 버튼
	{
		BitBlt(hdc,
			8, 434, 160, 160,
			hMemDC,
			0,0,
			SRCCOPY);
	}

	// 플레이어와 건물 색상: 녹색
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 150, 0));

	// 기존 브러시와 펜 저장
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	for_each(m_pPlayer->begin(), m_pPlayer->end(), [&](CObj* unit)
		{
			CreateRect(hdc,unit->Get_Info().fX, unit->Get_Info().fY, 5, false);
		});

	for_each(m_pBuild->begin(), m_pBuild->end(), [&](CObj* unit)
		{
			CreateRect(hdc, unit->Get_Info().fX, unit->Get_Info().fY, 6, true);
		});

	// 적과 적 건물 색상: 빨간색
	HBRUSH hEnemyBrush = CreateSolidBrush(RGB(0, 0, 180));
	SelectObject(hdc, hEnemyBrush); // 빨간색 브러시 적용

	for_each(m_pEnemy->begin(), m_pEnemy->end(), [&](CObj* unit)
		{
			CreateRect(hdc, unit->Get_Info().fX, unit->Get_Info().fY, 5, false);
		});

	for_each(m_pBuild_E->begin(), m_pBuild_E->end(), [&](CObj* unit)
		{
			CreateRect(hdc, unit->Get_Info().fX, unit->Get_Info().fY, 6, true);
		});

	// DC를 이전 상태로 복원
	SelectObject(hdc, hOldBrush);

	// 브러시 삭제
	DeleteObject(hBrush);
	DeleteObject(hEnemyBrush);

	HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	HPEN oldPen = (HPEN)SelectObject(hdc, newPen);
	HPEN oldBrush = (HPEN)SelectObject(hdc, newBrush);

	int x = CScrollMgr::Get_Instance()->Get_ScrollX();
	int y = CScrollMgr::Get_Instance()->Get_ScrollY();
	Rectangle(hdc, 
		(0	-	x) * Ratio + 8,
		(0	-	y) * Ratio + 434,
		(800 -	x) * Ratio + 8, 
		(600 -	y) * Ratio + 434);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);

	DeleteObject(newBrush);
	DeleteObject(newPen);
}

void CMiniMap::Release()
{
}

void CMiniMap::CreateRect(HDC hdc, float _x, float _y, int size, bool isBuild)
{
	RECT rc = {};

	if (!isBuild)
	{
		rc.left = LONG(_x * Ratio - (size * 0.5f) + 8);
		rc.top = LONG(_y * Ratio - (size * 0.5f) + 434);
		rc.right = LONG(_x * Ratio + (size * 0.5f) + 8);
		rc.bottom = LONG(_y * Ratio + (size * 0.5f) + 434);
	}
	else
	{
		rc.left = LONG(_x * Ratio -1 - (size * 0.5f) + 8);
		rc.top = LONG(_y * Ratio - (size * 0.5f) + 434);
		rc.right = LONG(_x * Ratio +1 + (size * 0.5f) + 8);
		rc.bottom = LONG(_y * Ratio + (size * 0.5f) + 434);
	}
	

	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}



