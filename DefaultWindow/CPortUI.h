#pragma once
#include "CUI.h"
#include "CObj.h"

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
	list<CObj*>* m_pUintlist;
	bool m_bRender;

	Port_Type m_ePreState;
	Port_Type m_eCurState;
};

