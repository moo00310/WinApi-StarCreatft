#pragma once
#include "Define.h"
#include "CObjMgr.h"


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


protected:
	INFO m_tInfo;
	RECT m_tRect;
	FRAME m_tFrame;
	const TCHAR* m_pImgKey;

protected:
	list<CObj*>* m_pUintlist;
	bool m_bRender;

	OBJ_TYPE m_ePreState;
	OBJ_TYPE m_eCurState;

};

