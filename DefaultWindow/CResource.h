#pragma once
#include "Define.h"

class CResource
{
public:
	CResource();
	~CResource() {}
public:
	void Initailize();
	void Update();
	void Render(HDC hDC);
	void Update_Rect() ;

public:
	const INFO* Get_Info_Pointer() { return &m_tInfo; }
	void SetPos(float x, float y) { m_tInfo.fX = x; m_tInfo.fY = y; }
protected:
	INFO		m_tInfo;
	RECT		m_tRect;


	int		m_iOption;
};

