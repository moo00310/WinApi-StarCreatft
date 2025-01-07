#pragma once
#include "CBuild.h"
class CCommedCenter : public CBuild
{
public:
	CCommedCenter(): m_bIsAddOn(false), m_Time(0){}
	~CCommedCenter() {}

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;

private:
	bool m_bIsAddOn;
	ULONGLONG m_Time;
};

