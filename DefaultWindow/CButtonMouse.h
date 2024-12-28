#pragma once
#include "CMouse.h"
class CButtonMouse : public CMouse
{
public:
	CButtonMouse() : m_indexY(0) 
	{
		ZeroMemory(&ptMouse, sizeof(POINT));
	}
	~CButtonMouse() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int m_indexY;
	POINT ptMouse;
};

