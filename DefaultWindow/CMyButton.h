#pragma once

#include "CObj.h"

class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	void Initialize()	override;
	int Update()		override;
	void Late_Update()	override;
	void Render(HDC hDC) override;
	void Release()		override;

private:
	int		m_iDrawID;


	// CObj을(를) 통해 상속됨
	void Move_Frame() override;

};

