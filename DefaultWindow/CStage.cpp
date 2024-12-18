#include "pch.h"
#include "CStage.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CMarine.h"
#include "CMapMgr.h"
#include "CObj.h"
#include "CGameMouse.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/MyTile/Tile.bmp", L"Tile");

	// 로드 타일
	CTileMgr::Get_Instance()->Load_Tile();
	// 타일 옵션 정보 불러오기 
	CMapMgr::Get_Instance()->Initialize_Map();
	
	// 마우스 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CGameMouse>::Create());

	// 마린 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create());

	// 적 마린 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMarine>::Create(600,600));
}

int CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();

	return 0;
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
}


void CStage::Render(HDC hDC)
{
	HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//BitBlt(hDC, iScrollX, iScrollY, 1920, 1280, hGroundDC, 0, 0, SRCCOPY);
	//CLineMgr::Get_Instance()->Render(hDC);

	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
}
