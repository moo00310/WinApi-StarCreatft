#pragma once
#include "CUnit.h"
class E_Tank : public CUnit
{
public:
	E_Tank() : m_eAttackDir(DIR_RIGHT), m_iBodyID(0), m_bSiegeMode(false), m_bSiegeMode_Anime(false)
	{
		ZeroMemory(&m_tBodyFram, sizeof(FRAME));
	}
	~E_Tank() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

	void DoSeigeMod()
	{
		m_bSiegeMode_Anime = true;
		SiegeMode();
	}

private:
	void Hold() override;
	void Move_toNext() override;
	void Update_State() override;

private:
	void MoveBody_Frame();
	void SiegeMode();
	void UnSiegeMode();
	void Frame_Init_Head(int start, int end, int time);
	void Frame_Init_Body(int start, int end, int time);

private:
	int m_iBodyID;
	bool m_bSiegeMode;
	bool m_bSiegeMode_Anime;
	DIRECTION m_eAttackDir;
	FRAME m_tBodyFram;
};

