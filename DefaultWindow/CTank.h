#pragma once
#include "CUnit.h"
class CTank : public CUnit
{
public:
	CTank(): m_eAttackDir(DIR_RIGHT){ ZeroMemory(&m_tAttackFrame, sizeof(FRAME)); }
	~CTank() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;

	virtual void Move();
	virtual void Attack();
	virtual void Hold();

	DIRECTION m_eAttackDir;
	FRAME m_tAttackFrame;
};

