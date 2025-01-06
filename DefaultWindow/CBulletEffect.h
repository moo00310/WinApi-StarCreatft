#pragma once
#include "CEffect.h"
class CBulletEffect : public CEffect
{
public:
	CBulletEffect() {}
	~CBulletEffect() {}
public:
	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() { return; };
	void Render(HDC hDC) PURE;
	void Release() {};
};

class CScvHit : public CBulletEffect
{
public:
	CScvHit() {}
	~CScvHit() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CMarineHit : public CBulletEffect
{
public:
	CMarineHit() {}
	~CMarineHit() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};


class CGhostHit : public CBulletEffect
{
public:
	CGhostHit() {}
	~CGhostHit() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CTankHit : public CBulletEffect
{
public:
	CTankHit() {}
	~CTankHit() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CSiegeTankHit : public CBulletEffect
{
public:
	CSiegeTankHit():m_bIsDamage(false), E_list(nullptr), E_build_list(nullptr)
	{}
	~CSiegeTankHit() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;

	int m_bIsDamage;
	list<CObj*>* E_list;
	list<CObj*>* E_build_list;


};

class CNukeMissile : public CBulletEffect
{
public:
	CNukeMissile() : BoomY(0) {}
	~CNukeMissile() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;

	int BoomY;
};

class CNukeMissileBoom : public CBulletEffect
{
public:
	CNukeMissileBoom() : m_bIsDamage(false), E_list(nullptr), E_build_list(nullptr)
	{}
	~CNukeMissileBoom() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;

	int m_bIsDamage;
	list<CObj*>* E_list;
	list<CObj*>* E_build_list;
};