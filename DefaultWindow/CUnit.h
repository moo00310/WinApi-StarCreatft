#pragma once
#include "CObj.h"

class CUnit : public CObj
{
public:
	CUnit(UNITID _id) : m_eUnitID(_id) { }
	~CUnit() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:
	// Move
	// Stop
	// Attack
	// Partrol


protected:
	UNITID m_eUnitID;
};

