#include "pch.h"
#include "CStage.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CMapMgr.h"
#include "CGameMouse.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CMouseMgr.h"
#include "CGameMgr.h"
#include "CResourceMgr.h"

#include "CCommedCenter.h"
#include "CBarrck.h"
#include "CSuffly.h"
#include "CRefinery.h"
#include "CFactory.h"
#include "CStarport.h"
#include "CAcademy.h"
#include "CArmory.h"
#include "CScienceFacility.h"


//// 테스트용
#include "CMarine.h"
#include "CScv.h"
#include "CMedic.h"
#include "CGhost.h"
#include "CTank.h"

#include "E_CommendCenter.h"
#include "E_Suffly.h"
#include "E_Refinery.h"
#include "E_Armory.h"
#include "E_Barrack.h"
#include "E_Academy.h"
#include "E_Factory.h"
#include "E_Starport.h"


#include "E_Marine.h"
#include "E_Medic.h"
#include "E_Ghost.h"
#include "E_Tank.h"



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

	//IMG
	Initalize_Bmp();
	
	//매니저 초기화
	CMapMgr::Get_Instance()->Initialize_Map(); 	// 타일& 리소스 옵션 정보 불러오기 
	CUIMgr::Get_Instance()->Initalize(); 	// UI 매니저 초기화 
	CGameMgr::Get_Instance()->Initialize();

	// 마우스 생산
	CMouseMgr::Get_Instance()->Add_Mouse(CAbstractFactory<CGameMouse>::Create());

	// 기초 건물 및 SCV 생산
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD, CAbstractFactory<CBarrck>::CreateBuild(300, 245));
		for (int i = 0; i < 4; i++)
		{
			//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CScv>::Create(250 + 25*i, 320));
		}
	}
	// 아군 유닛 생산
	//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(300, 300));
	//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMedic>::Create(300, 310));
	//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CGhost>::Create(300, 300));
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CTank>::Create(400, 400));

	// 적  생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Tank>::Create(600,600));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Medic>::Create(600,610));
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(4000,610));

	// 적 건물 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Academy>::CreateBuild(400, 600));
	
	
	
}

int CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	CGameMgr::Get_Instance()->Update();
	CMouseMgr::Get_Instance()->Update();

	return 0;
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
	CGameMgr::Get_Instance()->Late_Update();
	CMouseMgr::Get_Instance()->Late_Update();
}


void CStage::Render(HDC hDC)
{
	CMapMgr::Get_Instance()->Render(hDC);
	CResourceMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);
	CGameMgr::Get_Instance()->Render(hDC);
	CMouseMgr::Get_Instance()->Render(hDC);
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_BUILD);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_MONSTER);

}

void CStage::Initalize_Bmp()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/MyTile/Tile.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainUI.bmp", L"MainUI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");

	//Port
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/SCV/SCV.bmp", L"Scv_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Marine/Marine.bmp", L"Marine_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Medic/Medic.bmp", L"Medic_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Ghost/Ghost.bmp", L"Ghost_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/SeigeTank/Tank.bmp", L"Tank_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Advisor/Advosr.bmp", L"Advosr");

	//Wire
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Wire/BigWire.bmp", L"BigWire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Wire/SmallWire.bmp", L"SmallWire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/UpGrade_Icon/upgradeDetail.bmp", L"upgradeDetail");

	//Icon
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ETC.bmp", L"ETC");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");

	//Unit
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/SCV/SCV.bmp", L"Scv");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Marine/Marine.bmp", L"Marine");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Medic/Medic.bmp", L"Medic");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Ghost/Ghost.bmp", L"Ghost");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankBody.bmp", L"TankBody");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankHead.bmp", L"TankHead");

	//Select
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_0(32.32).bmp", L"Select_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_1(40.40).bmp", L"Select_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_3(64.64).bmp", L"Select_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_4(80.80).bmp", L"Select_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_5(96.96).bmp", L"Select_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_7(128.128).bmp", L"Select_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_8(148.148).bmp", L"Select_8");

	// Build_Temp
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/BuildTemplate.bmp", L"BuildTemplate");

	//Build
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/CommandCenter.bmp", L"CommandCenter");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/CmdNuke.bmp", L"CmdNuke");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/SupplyDepot.bmp", L"SupplyDepot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Refinery.bmp", L"Refinery");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Barracks.bmp", L"Barrck");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Academy.bmp", L"Academy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Factory.bmp", L"Factory");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/FactoryAddOn.bmp", L"FactoryAddOn");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Starport.bmp", L"Starport");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/StarportAddOn.bmp", L"StarportAddOn");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Armory.bmp", L"Armory");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/ScienceFacility.bmp", L"ScienceFacility");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/ScienceSecret.bmp", L"ScienceSecret");

	//Effect
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Bang/Bang(128.128).bmp", L"Bang_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/SCV/SCVEffect.bmp", L"SCVEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Marine/MarineGunSpark.bmp", L"MarineGunSpark");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Ghost/GhostShot.bmp", L"GhostShot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/TankAtt/TankHit.bmp", L"TankHit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/TankAtt/Tank(100,108).bmp", L"SiegeTankHit");

	//E_Unit
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/E_Unit/MarineBlue.bmp", L"MarineBlue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Medic/Medic_Blue.bmp", L"Medic_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Ghost/Ghost_Blue.bmp", L"Ghost_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankBody_Blue.bmp", L"TankBody_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankHead_Blue.bmp", L"TankHead_Blue");

	//E_Build
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/CommandCenter_Blue.bmp", L"CommandCenter_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Academy_Blue.bmp", L"Academy_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Armory_Blue.bmp", L"Armory_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Barracks_Blue.bmp", L"Barracks_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Factory_Blue.bmp", L"Factory_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Refinery_Blue.bmp", L"Refinery_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/Starport_Blue.bmp", L"Starport_Blue");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Enemy/SupplyDepot_Blue.bmp", L"SupplyDepot_Blue");


}
