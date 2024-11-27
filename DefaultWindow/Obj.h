#pragma once
#include "Define.h"

class Obj abstract
{
public:
	Obj();
	virtual ~Obj();

public:
	INFO GetInfo() { return m_tInfo; }
	const RECT* GetRect() { return &m_tRect; }
	void Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

public:
	virtual void		Initialize()PURE;
	virtual void		Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;

public:
	void Update_Rect();
	bool Collision(Obj* other)
	{
		RECT rc;
		return IntersectRect(&rc, this->GetRect(), other->GetRect()) == TRUE;
	}

protected:
	INFO m_tInfo;
	RECT m_tRect;

	float m_fSpeed;
};

