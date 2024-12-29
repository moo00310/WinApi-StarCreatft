#pragma once
#include "CUnit.h"
class CMedic : public CUnit
{
public:
	CMedic();
	~CMedic();
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

	void Move() override;
	void Hold() override;
	void Update_State() override;

	void HealUnit();
	CObj* preUint;
};

