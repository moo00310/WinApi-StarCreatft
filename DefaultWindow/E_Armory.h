#pragma once
#include "CBuild.h"
class E_Armory : public CBuild
{
public:
	E_Armory() {}
	~E_Armory() {}

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

