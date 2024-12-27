#pragma once
#include "CUnit.h"
class CScv : public CUnit
{
public:
	CScv();
	~CScv();
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;

	void AttackToEnemy(CObj* _Enemey);
private:

};

