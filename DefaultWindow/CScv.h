#pragma once
#include "CUnit.h"
class CScv : public CUnit
{
public:
	CScv():m_bBuildStructure(false), m_BuildAdvancedStructure(false) {}
	~CScv() {}
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
	bool m_bBuildStructure;
	bool m_BuildAdvancedStructure;

};

