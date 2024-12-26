#pragma once
#include "CUnit.h"

class CGhost : public CUnit
{
public:
	CGhost() {}
	~CGhost() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
};

