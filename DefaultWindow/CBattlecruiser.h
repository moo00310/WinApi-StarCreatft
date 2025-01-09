#pragma once
#include "CUnit.h"
class CBattlecruiser : public CUnit
{
public:
	CBattlecruiser():AttackCoolTime(0), m_isAttack(false), YamatoEnemy(nullptr), YamatoCount(0),
		m_Shade(nullptr), m_ColorMatrix{}, m_imgAttr{}
	{}
	~CBattlecruiser() {}

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Change_Motion() override;
	void KeyInput() override;
	void AttackToEnemy(CObj* _Enemey) override;

	void Update_State() override;
	bool CanGo(Pos pos) override;
	void Move_toNext() override;
	void SetEnemy(CObj* _enemy) { YamatoEnemy = _enemy; }

	void Yamato();

private:
	ULONGLONG AttackCoolTime;
	bool m_isAttack;
	CObj* YamatoEnemy;
	int YamatoCount;

	Gdiplus::Image* m_Shade;
	Gdiplus::ColorMatrix m_ColorMatrix;
	Gdiplus::ImageAttributes m_imgAttr;
};

