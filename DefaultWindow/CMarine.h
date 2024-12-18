#pragma once
#include "CUnit.h"
#include "Define.h"

class CMarine: public CUnit
{
public:
	CMarine();
	~CMarine();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Update_State() override;

private:
	void Change_Motion();
	void Test_Key_Input();

public:
	void MoveTo();

private:
	int m_iImgId;


};

