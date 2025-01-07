#pragma once
#include "CScene.h"
class CEnding : public CScene
{
public:
	CEnding() {}
	~CEnding() {}

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

