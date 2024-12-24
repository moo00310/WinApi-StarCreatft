#pragma once
#include "CEffect.h"

class CBloodEffect : public CEffect
{
public:
	CBloodEffect() : m_iCount(0), m_iDeadImg(0) {}
	~CBloodEffect() {}

	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() { return;  };
	void Render(HDC hDC) PURE;
	void Release() {};

protected:
	int m_iCount;
	int m_iDeadImg;
};

class CMarineDead : public CBloodEffect
{
public:
	CMarineDead() {};
	~CMarineDead() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CMedicDead : public CBloodEffect
{
public:
	CMedicDead() {};
	~CMedicDead() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CGhostDead : public CBloodEffect
{
public:
	CGhostDead() {};
	~CGhostDead() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};
