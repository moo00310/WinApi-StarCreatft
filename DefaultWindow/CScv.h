#pragma once
#include "CUnit.h"
class CScv : public CUnit
{
public:
	CScv():m_bBuildStructure(false), m_BuildAdvancedStructure(false), m_eBuildType(OT_END), isBuilding(false) ,
	m_iMyBuildTIme(0), m_iBuildCount(0), m_preDir(DIR_END)
	{
		ZeroMemory(&m_pos, sizeof(Pos));
	}
	~CScv() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;

public:
	void SetPos(Pos _pos) { m_pos = _pos; }
	bool GetBulid() { return m_bBuildStructure; }
	bool GetAdBuild() { return m_BuildAdvancedStructure; }

private:
	void Update_State() override;
	void AttackToEnemy(CObj* _Enemey);
	void SpwanBulid(OBJ_TYPE _type, Pos temp);

private:
	void Build();
	void BuildAime();
	bool GoToTarget(fPOINT targetPos);

private:
	bool m_bBuildStructure;
	bool m_BuildAdvancedStructure;

	OBJ_TYPE m_eBuildType;
	bool isBuilding;
	Pos m_pos;
	int m_iMyBuildTIme;
	int m_iBuildCount;

	DIRECTION m_preDir;
};

