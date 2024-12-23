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
	void Change_Port();
	void Anime_Port(int Start, int End);
};

