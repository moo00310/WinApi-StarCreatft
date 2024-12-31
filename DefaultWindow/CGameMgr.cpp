#include "pch.h"
#include "CGameMgr.h"
#include "CBmpMgr.h"

CGameMgr* CGameMgr::m_pInstance = nullptr;

void CGameMgr::Initialize()
{
	m_iMineral = 99999;
	m_iGas = 99999;
	m_iPop = 0;
	m_iMaxPop = 200;

	for (int i = 0; i < TECH_END; i++)
	{
		TechnicArray[i] = 1;
	}

	for (int i = 0; i < UG_END; ++i) {
		UpGrade_Compelate[i] = false;
	}
	int a =0;
}

void CGameMgr::Update()
{
	if (m_Time + 4000 < GetTickCount64())
	{
		m_iMineral += 10;
		m_iGas += 10;
		m_Time = GetTickCount64();
	}
}

void CGameMgr::Late_Update()
{
}

void CGameMgr::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ETC");

	BitBlt(hDC,
		500, 10, 14, 14,
		hMemDC,
		0,
		100,
		SRCCOPY);

	BitBlt(hDC,
		600, 10, 14, 14,
		hMemDC,
		15,
		100,
		SRCCOPY);

	BitBlt(hDC,
		700, 10, 14, 14,
		hMemDC,
		30,
		100,
		SRCCOPY);


	TextPrint(hDC);
}

void CGameMgr::Release()
{
}

bool CGameMgr::isBuying(OBJ_TYPE _Type)
{
	int _mineral = get<0>(ObjCost.at(_Type));
	int _gas = get<1>(ObjCost.at(_Type));
	int _pop = get<2>(ObjCost.at(_Type));

	if (m_iMineral >= _mineral && m_iGas >= _gas &&
		m_iMaxPop - m_iPop >= _pop)
	{
		m_iMineral -= _mineral;
		m_iGas -= _gas;
		m_iPop += _pop;

		return true;
	}
	else
	{
		if (m_iMineral < _mineral)
		{
			//미네랄이 부족합니다.
			return false;
		}
		else if (m_iGas < _gas)
		{
			//미네랄이 부족합니다.
			return false;
		}
		else
		{
			// 서플라이가 부족합니다
			return false;
		}
	}
}

void CGameMgr::TextPrint(HDC hdc)
{
	wchar_t	m_wcHp[32] = L"";
	// 텍스트 색상과 배경 설정
	SetTextColor(hdc, RGB(0, 200, 0));     // 텍스트 색상
	SetBkMode(hdc, TRANSPARENT);              // 배경 모드: 투명

	// 미네랄 텍스트 출력
	swprintf_s(m_wcHp, 32, L"%d ", m_iMineral);
	TextOut(hdc, 550, 10, m_wcHp, (int)wcslen(m_wcHp));

	// 가스 텍스트 출력
	swprintf_s(m_wcHp, 32, L"%d ", m_iGas);
	TextOut(hdc, 650, 10, m_wcHp, (int)wcslen(m_wcHp));

	// 인구수 텍스트 출력
	swprintf_s(m_wcHp, 32, L"%d / %d", (int)m_iPop, (int)m_iMaxPop);
	TextOut(hdc, 720, 10,  m_wcHp, (int)wcslen(m_wcHp));
}