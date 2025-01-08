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
#include "CKeyMgr.h"
#include "CScrollMgr.h"

//// 테스트용
#include "CMarine.h"
#include "CScv.h"
#include "CMedic.h"
#include "CGhost.h"
#include "CTank.h"
#include "CBattlecruiser.h"

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
	CSoundMgr::Get_Instance()->Stop_BGM();
	//BGM
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_terran_2.mp3", 0.3f); 

	//IMG
	Initalize_Bmp();
	
	//매니저 초기화
	CMapMgr::Get_Instance()->Initialize_Map(); 	// 타일& 리소스 옵션 정보 불러오기 
	CUIMgr::Get_Instance()->Initalize(); 	// UI 매니저 초기화 
	CGameMgr::Get_Instance()->Initialize();

	// 마우스 생산
	CMouseMgr::Get_Instance()->Add_Mouse(CAbstractFactory<CGameMouse>::Create());

	//MyObjSpwan();
	//EnemyObjSpwan();

	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CBattlecruiser>::Create(300.f, 300.f));

	// 승리 테스트
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Suffly>::CreateBuild(600.f, 600.f));
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(300.f, 300.f));
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(310.f, 310.f));
	}
	
}

int CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	CGameMgr::Get_Instance()->Update();
	CMouseMgr::Get_Instance()->Update();

	KeyInput();

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
	CObjMgr::Get_Instance()->Delete_ID(OBJ_BUILD_E);
}

void CStage::MyObjSpwan()
{
	// 기초 건물 및 SCV 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD, CAbstractFactory<CCommedCenter>::CreateBuild(300.f, 245.f));
	for (int i = 0; i < 4; i++)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CScv>::Create(250.f + 25 * i, 320.f));
	}
}

void CStage::EnemyObjSpwan()
{
	//// 적 건물 생산
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_CommendCenter>::CreateBuild(3810.f, 260.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_CommendCenter>::CreateBuild(2840.f, 430.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Refinery>::CreateBuild(3780.f, 100.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Refinery>::CreateBuild(2840.f, 300.f));

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Suffly>::CreateBuild(3940.f + 100 * j, 500.f + 60 * i));
		}
	}
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Suffly>::CreateBuild(3130.f + 100 * i, 50.f + 60 * j));
		}
	}

	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Academy>::CreateBuild(3470.f, 70.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Academy>::CreateBuild(3570.f, 70.f));

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 2; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Barrack>::CreateBuild(3450.f + 120 * i, 170.f + 100 * j));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Armory>::CreateBuild(3840.f, 500.f + 100 * i));
	}
	for (int i = 0; i < 4; i++)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Suffly>::CreateBuild(3840.f, 660.f + 60 * i));
	}


	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Factory>::CreateBuild(3520.f, 550.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Factory>::CreateBuild(3520.f, 650.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Factory>::CreateBuild(3520.f, 750.f));


	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Starport>::CreateBuild(3710.f, 650.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Starport>::CreateBuild(3710.f, 750.f));


	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Barrack>::CreateBuild(2860.f, 720.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUILD_E, CAbstractFactory<E_Suffly>::CreateBuild(2950.f, 680.f));


	// 적 방어 유닛 생산
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Tank>::CreateSiegeTank(3200.f, 600.f));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Tank>::CreateSiegeTank(2735, 583.f));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Tank>::CreateSiegeTank(3402, 644.f));


		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3015, 610));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3035, 620));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3005, 640.f));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3045, 610.f));

	}
}

void CStage::KeyInput()
{
	// SCV 정찰
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{

	}

	// 마린 메딕 러쉬 및 방어 부대
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		for (int i = 0; i < 8; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::CreateAttackEnemy(3600.f + 15 * i, 375.f, 320.f, 320.f));
		}
		for (int i = 0; i < 2; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Medic>::CreateAttackEnemy(3600.f, 375.f + 10 * i, 320.f, 320.f));
		}
	}

	// 마린 메딕 고스트 탱크 러쉬
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		for (int i = 0; i < 3; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Tank>::CreateAttackEnemy(3000.f + 10 * i, 820.f, 320.f, 320.f));
		}
		for (int i = 0; i < 8; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::CreateAttackEnemy(3000.f + 10 * i, 790.f, 320.f, 320.f));
		}
		for (int i = 0; i < 3; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Medic>::CreateAttackEnemy(3000.f + 10 * i, 800.f, 320.f, 320.f));
		}
		for (int i = 0; i < 2; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Ghost>::CreateAttackEnemy(3000.f + 10 * i, 810.f, 320.f, 320.f));
		}
		
	}

	// 방어 부대 소환
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
	{
		for (int i = 0; i < 4; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3600.f, 375.f +10 * i));
		}
		for (int i = 0; i < 2; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Medic>::Create(3600.f + 15 * i, 375.f));
		}

		for (int i = 0; i < 3; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3000.f, 480.f +10 * i));
		}
		for (int i = 0; i < 3; i++)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<E_Marine>::Create(3000.f +10 * i, 480.f));
		}
	}


}

void CStage::Initalize_Bmp()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/Texture/MyTile/Tile.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainUI.bmp", L"MainUI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");

	// minimap
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Map/MiniMap.bmp", L"MiniMap");

	//Port
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/SCV/SCV.bmp", L"Scv_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Marine/Marine.bmp", L"Marine_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Medic/Medic.bmp", L"Medic_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Ghost/Ghost.bmp", L"Ghost_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/SeigeTank/Tank.bmp", L"Tank_port");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Advisor/Advosr.bmp", L"Advosr");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Battlecruiser/Battle_port.bmp", L"Battle_port");

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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Battlecruiser/Battlecruiser(120,120).bmp", L"Battlecruiser");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Battlecruiser/BattleShade.bmp", L"BattleShade");

	//Select
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_0(32.32).bmp", L"Select_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_1(40.40).bmp", L"Select_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_3(64.64).bmp", L"Select_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_4(80.80).bmp", L"Select_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_5(96.96).bmp", L"Select_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_7(128.128).bmp", L"Select_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_8(148.148).bmp", L"Select_8");

	//Enemy_Select
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_0(32.32)_E.bmp", L"Select_0_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_1(40.40)_E.bmp", L"Select_1_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_3(64.64)_E.bmp", L"Select_3_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_4(80.80)_E.bmp", L"Select_4_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_5(96.96)_E.bmp", L"Select_5_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_7(128.128)_E.bmp", L"Select_7_E");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Enemy/Select_8(148.148)_E.bmp", L"Select_8_E");

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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Bang/Bang(252.200).bmp", L"Bang_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/SCV/SCVEffect.bmp", L"SCVEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Marine/MarineGunSpark.bmp", L"MarineGunSpark");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Ghost/GhostShot.bmp", L"GhostShot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/TankAtt/TankHit.bmp", L"TankHit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/TankAtt/Tank(100,108).bmp", L"SiegeTankHit");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/BattleAtk/BattleAttack.bmp", L"BattleAttack");
	
	//Nuke
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/NukeMissile/Nuke(48,40).bmp", L"NukeMissile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Nuke/Nuke_0_25(252.225).bmp", L"NukeBoom");

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

	//Build_Wreak
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Wreck/Wreck0(96.96).bmp", L"Wreck0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Wreck/Wreck1(128.128).bmp", L"Wreck1");

	//Build_Fire
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Fire/0_fire_0_11(64,96).bmp", L"fire_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Fire/1_fire_0_11(64.96).bmp", L"fire_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Effect/Fire/2_fire_0_11(64.96).bmp", L"fire_2");

	//WinText
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Win/WinText.bmp", L"WinText");

	//Dea
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Portrait/Dea.bmp", L"Dea");

}
