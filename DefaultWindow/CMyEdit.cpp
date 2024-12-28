#include "pch.h"
#include "CMyEdit.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CMouse.h"

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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/MyTile/Tile.bmp", L"Tile");
}

int CMyEdit::Update()
{
	CTileMgr::Get_Instance()->Update();
	m_pMouse->Update();
	dynamic_cast<EditMouse*>(m_pMouse)->GetEditInfo(m_eEditType,m_TileID);
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


	// 에디터 타입 변경
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		m_eEditType = ET_TILE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		m_eEditType = ET_GRASS;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		m_eEditType = ET_WALL;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
	{
		m_eEditType = ET_BRIGE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F5))
	{
		m_eEditType = ET_OBJECT;
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();


		if (m_eEditType <= ET_TILE)
		{
			CTileMgr::Get_Instance()->Picking_Tile(ptMouse, m_TileID, m_TileOption);
		}
		else
		{
			CTileMgr::Get_Instance()->Object_Tile(ptMouse, m_TileID, m_ObjectTile_iCX, m_ObjectTile_iCY, m_TileOption);
		}

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
	// 타일
	if (m_eEditType == ET_TILE)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_BACK))
		{
			m_TileID--;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			// 땅 타일
			ChangeTileID(TG_GROUND);
			m_TileOption = 0;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			// 언덕 타일
			ChangeTileID(TG_HILL);
			m_TileOption = 0;
		}
	}

	// 풀 타일
	if (m_eEditType == ET_GRASS)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			//땅 풀
			m_TileID = 28;
			m_ObjectTile_iCX = 10;
			m_ObjectTile_iCY = 6;
			m_TileOption = 0;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			// 언덕 풀
			m_TileID = 112;
			m_ObjectTile_iCX = 11;
			m_ObjectTile_iCY = 6;
			m_TileOption = 0;
		}
	}


	//벽 타일
	if (m_eEditType == ET_WALL)
	{

		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			// 벽 1
			m_TileID = 196;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			// 벽 2
			m_TileID = 200;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('3'))
		{
			// 벽 3
			m_TileID = 224;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('4'))
		{
			// 벽 4
			m_TileID = 228;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('5'))
		{
			// 벽 5
			m_TileID = 266;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 2;
			m_TileOption = 2;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('6'))
		{
			// 벽 5
			m_TileID = 270;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 2;
			m_TileOption = 2;
		}
	}

	// 다리 타일
	if (m_eEditType == ET_BRIGE)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			// 다리 1
			m_TileID = 294;
			m_ObjectTile_iCX = 6;
			m_ObjectTile_iCY = 6;
			m_TileOption = 1;
		}
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			// 다리 2
			m_TileID = 378;
			m_ObjectTile_iCX = 6;
			m_ObjectTile_iCY = 6;
			m_TileOption = 1;
		}
	}

	// 이동 불가 타일
	if (m_eEditType == ET_OBJECT)
	{
		// 땅 _공룡 뼈 타일
		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{
			m_TileID = 462;
			m_ObjectTile_iCX = 8;
			m_ObjectTile_iCY = 4;
			m_TileOption = 2;
		}

		// 언덕_구덩이
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{
			m_TileID = 518;
			m_ObjectTile_iCX = 4;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}

		// 언덕_건물_중
		if (CKeyMgr::Get_Instance()->Key_Down('3'))
		{
			m_TileID = 522;
			m_ObjectTile_iCX = 2;
			m_ObjectTile_iCY = 3;
			m_TileOption = 2;
		}

		// 언덕_건물_소
		if (CKeyMgr::Get_Instance()->Key_Down('4'))
		{
			m_TileID = 524;
			m_ObjectTile_iCX = 2;
			m_ObjectTile_iCY = 2;
			m_TileOption = 2;
		}

		// 언덕_뼈_소
		if (CKeyMgr::Get_Instance()->Key_Down('5'))
		{
			m_TileID = 552;
			m_ObjectTile_iCX = 2;
			m_ObjectTile_iCY = 1;
			m_TileOption = 2;
		}
	}
}

void CMyEdit::ChangeTileID(TILE_GROUP eGroup)
{
	int minID = TILE_ID_RANGES[eGroup][0];
	int maxID = TILE_ID_RANGES[eGroup][1];

	if (m_TileID < minID || m_TileID >= maxID)
		m_TileID = minID;
	else
		m_TileID++; 

	m_TileOption = 0;
}
