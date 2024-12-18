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

private:
	void Change_Motion();
	void Test_Key_Input();


private:
	int m_iImgId;


};

