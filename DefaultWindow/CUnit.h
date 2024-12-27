#pragma once
#include "CObj.h"
//#include "Define.h"

class CUnit : public CObj
{
public:
	CUnit() : m_pMonsterList(nullptr), m_iPathIndex(0), m_eInput(IP_ATTACK), m_iAttackFrame(0),
	ull_WaitTime(0), m_eCurState(STATE_IDLE), m_ePreState(STATE_IDLE)
		{ }
	~CUnit() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Change_Motion() PURE;
	virtual void KeyInput() PURE;
			void Move_Frame() override;
	virtual void AttackToEnemy(CObj* _Enemey) PURE;

public:
	void Update_State();
	void SetInput(INPUTSTATE _input) { m_eInput = _input; }
	INPUTSTATE GetInput() { return m_eInput; }
	ATIMESTATE GetAinmeState() { return m_eCurState; }

public:
	void Astar(Pos _tTarget_Index);
	bool CanGo(Pos pos);
	DIRECTION GetDirection(float player_x, float player_y, float monster_x, float monster_y);


protected:
	//Unit_BeHavior
	virtual void Move();
	virtual void Stop();
	virtual void Attack();
	virtual void Hold();

protected:
	INPUTSTATE m_eInput;
	int m_iAttackFrame;
	ATIMESTATE m_ePreState;
	ATIMESTATE m_eCurState;

	//A_star
	vector<Pos>	_path;
	int m_iPathIndex;
	
	//Attack
	list<CObj*>* m_pMonsterList;

	//Move
	ULONGLONG	ull_WaitTime;
};

