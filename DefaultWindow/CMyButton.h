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

};

