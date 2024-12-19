#pragma once
#include "CUI.h"
#include "CUnit.h"

class CWireUI : public CUI
{
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_Port();

private:
	list<CUnit*>* m_pUintlist;
	UNITID m_eID;
	bool m_bRender;

};

