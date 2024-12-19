#pragma once
#include "CUI.h"
#include "CUnit.h"

class CWireUI : public CUI
{
public:
	CWireUI();
	~CWireUI();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void Change_Port();
	void TextPrint(HDC hdc);
	void TextName(HDC hdc);

private:
	list<CObj*>* m_pUintlist;
	UNITID m_eID;
	bool m_bRender;

	float MaxHp;
	float NowHp;
};

