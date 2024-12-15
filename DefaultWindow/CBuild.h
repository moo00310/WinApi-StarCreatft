#pragma once
#include "CObj.h"

class CBuild : public CObj
{
public:
	CBuild() {}
	~CBuild() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

};

