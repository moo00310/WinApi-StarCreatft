#pragma once
#include "CUnit.h"
class CBattlecruiser : public CUnit
{
public:
	CBattlecruiser():AttackCoolTime(0), m_isAttack(false) {}
	~CBattlecruiser() {}

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;


	bool CanGo(Pos pos) override;

private:
	ULONGLONG AttackCoolTime;
	bool m_isAttack;
};

