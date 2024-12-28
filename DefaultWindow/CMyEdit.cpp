#include "pch.h"
#include "CMyEdit.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CMouse.h"
#include "CMapMgr.h"

CMyEdit::CMyEdit(): m_TileID(0), m_TileOption(0), m_pMouse(nullptr), m_eEditType(ET_END)
, m_ObjectTile_iCX(0), m_ObjectTile_iCY(0)
{
}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CTileMgr::Get_Instance()->Initialize();

	m_pMouse = new EditMouse();
	m_pMouse->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/Tilesquare.bmp", L"Tilesquare");
}

int CMyEdit::Update()
{
	CTileMgr::Get_Instance()->Update();
	m_pMouse->Update();
	dynamic_cast<EditMouse*>(m_pMouse)->GetEditInfo(m_TileOption);
	return 0;
}

void CMyEdit::Late_Update()
{
	Key_Input();
	m_pMouse->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC hDC)
{
	CMapMgr::Get_Instance()->Render(hDC);
	CTileMgr::Get_Instance()->Render(hDC);
	m_pMouse->Render(hDC);

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//그리드 그리기
	for (int i = 0; i < 128; ++i)
	{
		MoveToEx(hDC, int(i * 32 + fScrollX), int(0 + fScrollY), nullptr);
		LineTo(hDC, int(i * 32 + fScrollX), int(128 * 32 + fScrollY));
	}
	for (int i = 0; i < 128; ++i)
	{
		MoveToEx(hDC, int(0 + fScrollX), int(i * 32 + fScrollY), nullptr);
		LineTo(hDC, int(128 * 32 + fScrollX), int(i * 32 + fScrollY));
	}
}

void CMyEdit::Release()
{
	Safe_Delete<CObj*>(m_pMouse);
}

void CMyEdit::Key_Input()
{
	// 이동
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();


		CTileMgr::Get_Instance()->Picking_Tile(ptMouse, m_TileOption);

	}

	// 타일 변경
	TileChange();
	
	
	// 저장 / 불러오기
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		CTileMgr::Get_Instance()->Load_Tile();
		return;
	}
}

void CMyEdit::TileChange()
{
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		// 이동가능
		m_TileOption = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		//  언덕
		m_TileOption = 1;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		// 이동불가
		m_TileOption = 2;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		// 가스
		m_TileOption = 3;
	}
}