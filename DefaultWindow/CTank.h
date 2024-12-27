#pragma once
#include "CUnit.h"
class CTank : public CUnit
{
public:
	CTank(): m_eAttackDir(DIR_RIGHT), m_iBodyID(0), dwMoveTime(GetTickCount64()), m_bSiegeMode(false), SiegeCount(0),
	m_bSiegeMode_Anime(false)
	{
		ZeroMemory(&m_tBodyFram, sizeof(FRAME));
	}
	~CTank() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;

	void AttackToEnemy(CObj* _Enemey) override;
	virtual void Move();
	virtual void Attack();
	virtual void Hold();

private:
	void MoveBody_Frame();
	void SiegeMode();
	void UnSiegeMode();
	void Frame_Init_Head(int start, int end, int time);
	void Frame_Init_Body(int start, int end, int time);

private:
	int SiegeCount;
	ULONGLONG dwMoveTime;
	int m_iBodyID;
	bool m_bSiegeMode;
	bool m_bSiegeMode_Anime;
	DIRECTION m_eAttackDir;
	FRAME m_tBodyFram;
};

