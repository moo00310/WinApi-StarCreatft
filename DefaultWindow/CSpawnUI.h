#pragma once
#include "CUI.h"
#include "CObj.h"

class CSpawnUI : public CUI
{
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_SpawnIcon(OBJ_TYPE _type);


private:
	queue<OBJ_TYPE>* m_queSpawn;
	queue<OBJ_TYPE> m_showQue;
	OBJ_TYPE m_eID;

};

