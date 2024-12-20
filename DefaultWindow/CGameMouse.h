#pragma once
#include "CMouse.h"
#include "CUnit.h"
#include "CObj.h"

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

public:
	void ClearList();

private:
	void MouseInput(POINT ptMouse);
	void ScrollMove(POINT mouse);
	void ColObject();
	void Change_Cursor();

private:
	POINT ptMouse{};
	CURSERSTATE m_eCurState;
	CURSERSTATE m_ePreState;
	int m_indexY;
	list<CObj*>* m_UnitList;
	list<CObj*>* m_Select_UnitList;
	CObj* m_pPickObj;
};

