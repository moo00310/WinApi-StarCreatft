#pragma once
#include "CUnit.h"

class CGhost : public CUnit
{
public:
	CGhost() : AttackCoolTime(0), m_isAttack(false), m_fCloack(0.f), m_Ghost(nullptr), m_ColorMatrix{}, m_imgAttr{}, m_bIsCloack(false)
	{}
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

	bool GetCloack() { return m_bIsCloack; }


private:
	ULONGLONG AttackCoolTime;
	int m_isAttack;
	Pos m_Nuketarget;
	float m_fCloack;
	bool m_bIsCloack;

	Gdiplus::Image* m_Ghost;
	Gdiplus::ColorMatrix m_ColorMatrix;
	Gdiplus::ImageAttributes m_imgAttr;
};

