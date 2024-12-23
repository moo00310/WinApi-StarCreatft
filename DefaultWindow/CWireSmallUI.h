#pragma once
#include "CUI.h"
#include "CObj.h"

class CWireSmallUI : public CUI
{
public:
	CWireSmallUI();
	~CWireSmallUI();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_Wire(CObj* unit);

private:
	OBJ_TYPE m_eID;
	int iCount;

	size_t m_iUnitCount;
	float MaxHp;
	float NowHp;
};

