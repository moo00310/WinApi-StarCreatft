#pragma once
#include "CUI.h"
#include "CObj.h"

class CIconUI :public CUI
{
public:
	CIconUI();
	~CIconUI();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_Button();

private:
	void SCV();
	void Marine();
	void Medic();
	void Ghost();
	void Tank();
	void SiegeTank();

	//--------------------------------
	void CommedCenter();
	void Suffly();
	void Refinery();
	void Barrack();
	void Archerdemy();
	void Factory();
	void Starport();
	void Armory();
	void Siencefacilly();

	//------------------------------
	void FactoryAddon();
	void StarportAddon();
	void CovertOps();
	void cmdNuke();



private:
	int IconCX;
	int IconCY;

	INPUTSTATE m_InputState;
	BuildSTATE m_buildState;
	ATIMESTATE m_UnitState;

	pair<int, int> m_Button_Icon[9];

public:
	enum ICON
	{
		IC_Move,
		IC_Stop,
		IC_Attack,
		IC_Patrol,
		IC_Hold,
		IC_Cancel,
		IC_Return,
		IC_Gater,
		IC_Refair,
		IC_Build,
		IC_AdBUild,
		//--------
		IC_Scv,
		//-------
		IC_Marine,
		IC_firebat,
		IC_Ghost,
		//-------
		IC_Vulture,
		IC_Tank,
		IC_Goliath,
		//--------
		IC_Wirse,
		IC_Dropship,
		IC_BattleCulsor,
		IC_Vassle,
		//--------------
		IC_Commend,
		IC_Suffly,
		IC_Refinery,
		IC_Barrack,
		IC_Enginer,
		IC_Terret,
		IC_Archerdemy,
		IC_Bungker,
		//---------
		IC_Factory,
		IC_Starport,
		IC_Siencefacilly,
		IC_Armory,
		//-------------
		IC_Scan_Addon,
		IC_Nuke_Addon,
		IC_CoOvp_Addon,
		IC_BattleAddon,
		IC_StarportAddon,
		IC_FactoryAddon,
		//--------------
		IC_Siegemod,
		IC_Tankmod,
		//-------------
		IC_Medic,
		IC_Vakily,
		//----------------
		IC_MarineRichUp,
		IC_MarineSteamPack,
		IC_MedicMegic_1,
		IC_MedicMegic_2,
		IC_MedicMana,
		//--------------
		IC_Boinic_AtkUp,
		IC_Boinic_DefUp,
		//-------------
		IC_Mecha_AtkUp,
		IC_Sky_AtkUp,
		IC_Mecha_DefUp,
		IC_Sky_DefUp,
		//-------------
		IC_Vasse_Emp,
		IC_Vasse_Eraady,
		IC_Vasse_Mana,
		//--------------
		IC_Wirse_Cloak,
		IC_Wirse_Decloak,
		IC_Wirse_Mana,
		//---------------
		IC_Vulture_SpeedUp,
		IC_Vulture_MineUp,
		IC_Tank_SiegeUp,
		IC_Goliath_SightUp,
		//----------------
		IC_Battle_AmatoUp,
		IC_Battle_ManaUp,
		//---------------
		IC_Cmd_NukeUp,
		IC_Ghost_LockDownUp,
		IC_Ghost_CloakUp,
		IC_Ghost_SightUp,
		IC_Ghost_Mana,
		IC_Cmp_Scan,
		//-------------------
		IC_Ghost_Cloak,
		IC_Ghost_LockDown,
		IC_Ghost_Nuke,
		//------
		IC_Battle_Amato
	};
};

