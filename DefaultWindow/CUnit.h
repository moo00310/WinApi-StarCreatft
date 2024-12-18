#pragma once
#include "CObj.h"

class CUnit : public CObj
{
public:
	CUnit(UNITID _id) : m_eUnitID(_id), m_Map(nullptr), m_iPathIndex(0) { }
	~CUnit() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:
	virtual void Update_State() PURE;

public:
	// Move
	void Astar(Pos _tTarget_Index);
	bool CanGo(Pos pos);

	// Stop


	// Attack
	void Attack();

	// Partrol


protected:
	UNITID m_eUnitID;
	ATIMESTATE m_ePreState;
	ATIMESTATE m_eCurState;

	//A_star
	vector<vector<int>>* m_Map;
	vector<Pos>	_path;
	int m_iPathIndex;
	

	//Attack
	list<CObj*> m_pMonsterList;


};

