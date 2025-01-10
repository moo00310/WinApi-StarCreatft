#pragma once
#include "CBuild.h"
class E_Starport : public CBuild
{
public:
	E_Starport() { FireRemove(); }
	~E_Starport() {}

	// CBuild을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

