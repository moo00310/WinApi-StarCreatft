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

	bool ChekList_OBJ(OBJ_TYPE _id);

private:
	int IconCX;
	int IconCY;

	INPUTSTATE m_InputState;
	BuildSTATE m_buildState;
	ATIMESTATE m_UnitState;

	pair<int, int> m_Button_Icon[9];
	list<OBJ_TYPE>* m_listSpawn;
public:
	
};

