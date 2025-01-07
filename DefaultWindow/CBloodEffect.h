#pragma once
#include "CEffect.h"

class CBloodEffect : public CEffect
{
public:
	CBloodEffect() {}
	~CBloodEffect() {}

	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() { return;  };
	void Render(HDC hDC) PURE;
	void Release() {};
};

class CSCVDead : public CBloodEffect
{
public:
	CSCVDead() {};
	~CSCVDead() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
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


class CBuildDead : public CBloodEffect
{
public:
	CBuildDead() {};
	~CBuildDead() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CBuildDead_Wreck_Small : public CBloodEffect
{
public:
	CBuildDead_Wreck_Small() {};
	~CBuildDead_Wreck_Small() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CBuildDead_Wreck_Big : public CBloodEffect
{
public:
	CBuildDead_Wreck_Big() {};
	~CBuildDead_Wreck_Big() {};

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};
