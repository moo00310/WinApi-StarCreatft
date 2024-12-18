#pragma once
#include "CMouse.h"

class CGameMouse : public CMouse
{
public:
	CGameMouse();
	~CGameMouse();
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void MouseInput(POINT ptMouse);
	void ScrollMove(POINT mouse);
	void Change_Cursor();

private:
	CURSERSTATE m_eCurState;
	CURSERSTATE m_ePreState;
	int m_indexY;
};

