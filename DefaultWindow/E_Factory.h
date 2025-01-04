#pragma once
#include "CBuild.h"
class E_Factory : public CBuild
{
public:
	E_Factory() {}
	~E_Factory() {}

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

