#pragma once
#include "CEffect.h"

class CBloodEffect : public CEffect
{
public:
	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() PURE;
	void Render(HDC hDC) PURE;
	void Release() PURE;
};

class CMarineDead : public CBloodEffect
{
public:
	CMarineDead();
	~CMarineDead();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	int m_iCount;
	int m_iDeadImg;
};

