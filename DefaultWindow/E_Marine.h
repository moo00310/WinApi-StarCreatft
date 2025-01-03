#pragma once
#include "CUnit.h"

class E_Marine : public CUnit
{
public:
	E_Marine():m_dwTime(GetTickCount64()){}
	~E_Marine() {}

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

	ULONGLONG m_dwTime;
};

