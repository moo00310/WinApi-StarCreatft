#pragma once
#include "CUI.h"
#include "CObj.h"

class CSpawnUI : public CUI
{
public:
	CSpawnUI();
	~CSpawnUI();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_SpawnIcon(OBJ_TYPE _type);

private:
	list<OBJ_TYPE>* m_SpawnList;
	OBJ_TYPE m_eID;

	int IndexX;
	int IndexY;
};

