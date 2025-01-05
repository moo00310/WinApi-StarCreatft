#pragma once
#include "CUnit.h"
class E_Medic : public CUnit
{
public:
	E_Medic() {}
	~E_Medic() {}

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

	void Move() override;
	void Update_State() override;

	void HealUnit();
	void Attack() override;

};

