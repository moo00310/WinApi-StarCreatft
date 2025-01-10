#pragma once
#include "CBuild.h"
class E_CommendCenter : public CBuild
{
public:
	E_CommendCenter() {}
	~E_CommendCenter() { FireRemove(); }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

