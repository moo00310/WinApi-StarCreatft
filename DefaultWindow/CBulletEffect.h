#pragma once
#include "CEffect.h"
class CBulletEffect : public CEffect
{
public:
	CBulletEffect() {}
	~CBulletEffect() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

