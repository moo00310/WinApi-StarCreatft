#pragma once
#include "Define.h"

class CResource
{
public:
	CResource();
	~CResource() {}
public:
	void Update();
	void Render(HDC hDC);
	void Update_Rect();

public:
	const INFO* Get_Info_Pointer() { return &m_tInfo; }
	RECT GetRect() { return m_tRect; }
	void SetPos(float x, float y) { m_tInfo.fX = x; m_tInfo.fY = y; }
	void SetOption(int num);
	int GetOption() { return m_iOption; }

protected:
	INFO		m_tInfo;
	RECT		m_tRect;

	int		m_iOption;
};

