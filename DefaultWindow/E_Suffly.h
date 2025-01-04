#pragma once
#include "CBuild.h"
class E_Suffly : public CBuild
{
public:
	E_Suffly() {}
	~E_Suffly(){}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

