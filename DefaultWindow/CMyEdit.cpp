#include "pch.h"
#include "CMyEdit.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CMouse.h"

CMyEdit::CMyEdit(): m_TileID(0), m_TileOption(0), m_pMouse(nullptr), m_eEditType(ET_END)
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
	if (CKeyMgr::Get_Instance()->Key_Down('T'))
	{
		m_eEditType = ET_TILE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('O'))
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

		switch (m_eEditType)
		{
		case ET_TILE:
			CTileMgr::Get_Instance()->Picking_Tile(ptMouse, m_TileID, m_TileOption);
			break;
		case ET_OBJECT:
			break;
		case ET_END:
			break;
		default:
			break;
		}
	}

	// 타일
	if (m_eEditType == ET_TILE)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('1'))
		{

			if (m_TileID < 0 || m_TileID >= 13)
				m_TileID = 0;
			else
				m_TileID++;

			m_TileOption = 0;

		}
		if (CKeyMgr::Get_Instance()->Key_Down('2'))
		{

			if (m_TileID < 14 || m_TileID >= 27)
				m_TileID = 14;
			else
				m_TileID++;

			m_TileOption = 0;
		}
	}
	

	//오브젝트
	if (m_eEditType == ET_OBJECT)
	{

	}
	
	
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
