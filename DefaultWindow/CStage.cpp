#include "pch.h"
#include "CStage.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CMapMgr.h"
#include "CGameMouse.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"



//// 테스트용
#include "CMarine.h"
#include "CBarrck.h"
#include "CScv.h"
#include "CMedic.h"
#include "CGhost.h"
#include "CTank.h"

float	g_fVolume(1.f);

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	//BGM
	//CSoundMgr::Get_Instance()->PlayBGM(L"BGM_terran_2.mp3", 0.6f); 

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/MyTile/Tile.bmp", L"Tile");

	//매니저 초기화
	CTileMgr::Get_Instance()->Load_Tile(); // 로드 타일
	CMapMgr::Get_Instance()->Initialize_Map(); 	// 타일 옵션 정보 불러오기 
	CUIMgr::Get_Instance()->Initalize(); 	// UI 매니저 초기화 

	// 마우스 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CGameMouse>::Create());

	// 적 마린 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMarine>::Create(600,600));

	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CTank>::Create(400, 400));
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CScv>::Create(300, 300));
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CGhost>::Create(300, 300));


}

int CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();

	return 0;
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
}


void CStage::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_MOUSE);
}
