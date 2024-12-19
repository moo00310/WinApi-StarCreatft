#pragma once
#include "CUI.h"

class CMainUI : public CUI
{
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;
};

