#pragma once
#include "CUnit.h"
class E_Ghost : public CUnit
{
public:
	E_Ghost() {}
	~E_Ghost() {}

	// CUnit을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

private:
	ULONGLONG AttackCoolTime;
	int m_isAttack;
};

