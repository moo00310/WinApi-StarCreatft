#pragma once
#include "CBuild.h"

class CBarrck : public CBuild
{

public:
	CBarrck();
	~CBarrck();
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void Change_Motion();


};

