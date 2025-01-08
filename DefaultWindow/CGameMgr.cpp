#include "pch.h"
#include "CGameMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CMapMgr.h"
#include "CSoundMgr.h"
#include "CObjMgr.h"
#include "CSceneMgr.h"
#include "CScrollMgr.h"

CGameMgr* CGameMgr::m_pInstance = nullptr;

void CGameMgr::Initialize()
{
	m_iMineral = 9999;
	m_iGas = 9999;
	m_iPop = 4;
	m_iMaxPop = 10;

	for (int i = 0; i < TECH_END; i++)
	{
		TechnicArray[i] = 0;
	}

	for (int i = 0; i < UG_END; ++i) {
		UpGrade_Compelate[i] = false;
	}

}

void CGameMgr::Update()
{

	// 맵 디버그
	/*if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		m_isDeBug = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		m_isDeBug = false;
	}*/

	if(CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-10.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(10.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(10.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-10.f);
}

void CGameMgr::Late_Update()
{
	if (m_isWin && CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		CSoundMgr::Get_Instance()->PlaySFX(L"mousedown2.wav", 0.6f);
		CSceneMgr::Get_Instance()->Set_Scene(SC_ENDING);
	}
}

void CGameMgr::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ETC");
	HDC		hWinDC = CBmpMgr::Get_Instance()->Find_Image(L"WinText");


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

#pragma region 맵 디버그
	//if (m_isDeBug)
	//{
	//	//그리드 그리기
	//	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	//	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//	for (int i = 0; i < 128; ++i)
	//	{
	//		MoveToEx(hDC, int(i * 32 + fScrollX), int(0 + fScrollY), nullptr);
	//		LineTo(hDC, int(i * 32 + fScrollX), int(128 * 32 + fScrollY));
	//	}
	//	for (int i = 0; i < 128; ++i)
	//	{
	//		MoveToEx(hDC, int(0 + fScrollX), int(i * 32 + fScrollY), nullptr);
	//		LineTo(hDC, int(128 * 32 + fScrollX), int(i * 32 + fScrollY));
	//	}

	//	// 화면에 보이는 타일의 인덱스 범위 계산
	//	int startX = max(0, int(-fScrollX / TILECX));
	//	int startY = max(0, int(-fScrollY / TILECY));
	//	int endX = min(128, startX + WINCX / TILECX + 2);
	//	int endY = min(128, startY + WINCY / TILECY + 2);

	//	for (int i = startY; i < endY; ++i) {
	//		for (int j = startX; j < endX; ++j) {
	//			Pos temp = { i, j };

	//			// 타일 정보 가져오기
	//			wchar_t m_wcHp[32] = L"";
	//			swprintf_s(m_wcHp, 32, L"%d ", CMapMgr::Get_Instance()->GetTileType(temp));

	//			// 화면 좌표 계산 (스크롤 오프셋 적용)
	//			int screenX = j * TILECX + fScrollX;
	//			int screenY = i * TILECY + fScrollY;

	//			// 텍스트 출력
	//			TextOut(hDC, screenX + TILECX / 2, screenY + TILECY / 2, m_wcHp, (int)wcslen(m_wcHp));
	//		}
	//	}
	//}
#pragma endregion

	if (CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E)->size() <= 0)
	{
		if (!m_isWin)
		{
			CSoundMgr::Get_Instance()->Stop_BGM();
			CSoundMgr::Get_Instance()->Stop_SFX();
			m_Time = GetTickCount64();
			//효과음
			CSoundMgr::Get_Instance()->PlaySFX(L"WinSound1.mp3", 0.8f);
			m_isWin = true;
		}
		
		wchar_t	m_wcHp[32] = L"";
		// 텍스트 색상과 배경 설정
		SetTextColor(hDC, RGB(255, 242, 0));     // 텍스트 색상
		SetBkMode(hDC, TRANSPARENT);              // 배경 모드: 투명

		// 미네랄 텍스트 출력
		swprintf_s(m_wcHp, 32, L"플레이어 1 패배");
		TextOut(hDC, 30, 360, m_wcHp, (int)wcslen(m_wcHp));

		
		if (m_Time + 3000 < GetTickCount64())
		{
			// 승리
			BitBlt(hDC,
				200, 200, 400, 180,
				hWinDC,
				0,
				0,
				SRCCOPY);

			if (!m_isSound)
			{
				CSoundMgr::Get_Instance()->PlaySFX(L"WinSound2.mp3", 0.8f);
				m_isSound = true;
			}
			
		}
	}
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
			CSoundMgr::Get_Instance()->PlaySFX(L"ErrMineral.mp3", 0.3f);
			
			return false;
		}
		else if (m_iGas < _gas)
		{
			CSoundMgr::Get_Instance()->PlaySFX(L"ErrGas.mp3", 0.3f);

			return false;
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySFX(L"ErrSupply.mp3", 0.3f);
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