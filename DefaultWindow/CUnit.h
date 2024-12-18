#pragma once
#include "CObj.h"

class CUnit : public CObj
{
public:
	CUnit(UNITID _id) : m_eUnitID(_id), m_Map(nullptr), m_iPathIndex(0),
	m_pMonsterList(nullptr), m_eCurState(STATE_IDLE), m_ePreState(STATE_IDLE), m_eInput(IP_END) { }
	~CUnit() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:
	void Update_State();
	void SetInput(INPUTSTATE _input) { m_eInput = _input; }

public:
	void Astar(Pos _tTarget_Index);
	bool CanGo(Pos pos);
	DIRECTION GetDirection(float player_x, float player_y, float monster_x, float monster_y);

	// Move
	void Move();

	// Stop
	void Stop();

	// Attack
	void Attack();

	// Hold
	void Hold();

protected:
	UNITID m_eUnitID;
	ATIMESTATE m_ePreState;
	ATIMESTATE m_eCurState;
	INPUTSTATE m_eInput;

	//A_star
	vector<vector<int>>* m_Map;
	vector<Pos>	_path;
	int m_iPathIndex;
	

	//Attack
	list<CObj*>* m_pMonsterList;

};

