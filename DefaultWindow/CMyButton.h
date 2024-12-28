#pragma once

#include "CObj.h"

class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	void Initialize()	override;
	virtual int Update()override;
	void Late_Update()	override;
	void Render(HDC hDC) override;
	void Release()		override;
	void Move_Frame() override;

	void Update_Button_Rect();

	INFO m_tButton;
	RECT m_tButtonRect;

	int m_iCount;
	const TCHAR* m_Name;
};

