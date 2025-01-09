#pragma once
#include "CUnit.h"
class E_Ghost : public CUnit
{
public:
	E_Ghost():AttackCoolTime(0), m_isAttack(false), m_bIsCloack(false){}
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

	bool GetCloack() { return m_bIsCloack; }

private:
	bool m_bIsCloack;
	ULONGLONG AttackCoolTime;
	int m_isAttack;
};

