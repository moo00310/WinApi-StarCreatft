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
	void KeyInput() override;

private:
	void Change_Motion();
	void Block_Map();
	void UnBlock_Map();


};

