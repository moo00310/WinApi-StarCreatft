#pragma once
#include "CBuild.h"
class CStarport : public CBuild
{
public:
	CStarport() {}
	~CStarport() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;
};

