#pragma once
#pragma once
#include "Define.h"

class CUI
{
public:
	CUI();
	virtual ~CUI() {}

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release()PURE;

public:
	void Update_Rect();
	void Move_Frame();


public:
	INFO m_tInfo;
	RECT m_tRect;
	FRAME m_tFrame;
	const TCHAR* m_pImgKey;

};

