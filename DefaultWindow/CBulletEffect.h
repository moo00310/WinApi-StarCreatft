#pragma once
#include "CEffect.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
class CBulletEffect : public CEffect
{
public:
	CBulletEffect():m_iFire(0) {}
	~CBulletEffect() {}
public:
	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() { return; };
	void Render(HDC hDC) PURE;
	void Release() {};
	virtual void Change_Motion() {};

	void SetFire(int num) { m_iFire = num; }
	int GetFire() { return m_iFire; }

protected:
	int m_iFire;
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

class CSiegeTankHit_E : public CBulletEffect
{
public:
	CSiegeTankHit_E() :m_bIsDamage(false), E_list(nullptr), E_build_list(nullptr)
	{}
	~CSiegeTankHit_E() {}
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
	CNukeMissileBoom() : m_bIsDamage(false), E_list(nullptr), E_build_list(nullptr), m_NukeBoom(nullptr), m_ColorMatrix{},
		m_imgAttr{}
	{
	}
	~CNukeMissileBoom() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;

	int m_bIsDamage;
	list<CObj*>* E_list;
	list<CObj*>* E_build_list;
	float m_fTransitionAlpha;

	Gdiplus::Image* m_NukeBoom;
	Gdiplus::ColorMatrix m_ColorMatrix;
	Gdiplus::ImageAttributes m_imgAttr;
};

class CBuildFire_0 : public CBulletEffect
{
public:
	CBuildFire_0(){}
	~CBuildFire_0() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CBuildFire_1 : public CBulletEffect
{
public:
	CBuildFire_1() {}
	~CBuildFire_1() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CBuildFire_2 : public CBulletEffect
{
public:
	CBuildFire_2() {}
	~CBuildFire_2() {}
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

class CBattleAttack : public CBulletEffect
{
public:
	CBattleAttack(CObj* _my, CObj* _Enemy) : m_Enemy(_Enemy), m_my(_my), m_bIsDamage(false), E_build_list(nullptr), E_list(nullptr), Ladian(0)
	{}
	~CBattleAttack() {}

public:
	void Initialize();
	int Update() override;
	void Render(HDC hDC) override;

	int m_bIsDamage;
	CObj* m_Enemy;
	CObj* m_my;
	list<CObj*>* E_list;
	list<CObj*>* E_build_list;
	float Ladian;
};