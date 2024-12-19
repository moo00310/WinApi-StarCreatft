#pragma once
#include "CUI.h"
#include "CUnit.h"

class CPortUI :public CUI
{
public:
	CPortUI();
	~CPortUI();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;
private:
	void Initailize_Img();
	void Change_Port();
	void Anime_Port(int Start, int End);

private:
	list<CUnit*>* m_pUintlist;
	UNITID m_eID;
	bool m_bRender;
};

