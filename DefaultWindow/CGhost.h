#pragma once
#include "CUnit.h"

class CGhost : public CUnit
{
public:
	CGhost(): AttackCoolTime(0), m_isAttack(false) {}
	~CGhost() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;

	void AttackToEnemy(CObj* _Enemey) override;
	void Update_State() override;

	void Nuke();
	void SetNukePos(Pos pos) { m_Nuketarget = pos; }

private:
	ULONGLONG AttackCoolTime;
	int m_isAttack;
	Pos m_Nuketarget;
};

