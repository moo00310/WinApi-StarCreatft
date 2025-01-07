#pragma once
#include "CBuild.h"
class CRefinery : public CBuild
{
public:
	CRefinery():m_Time(0) {}
	~CRefinery() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;

private:
	ULONGLONG m_Time;
};

