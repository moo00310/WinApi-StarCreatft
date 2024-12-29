#pragma once
#include "CBuild.h"
class CFactory : public CBuild
{
public:
	CFactory(): m_bIsAddOn(false), m_pAddOn(nullptr), m_bBuildAddon(false){}
	~CFactory() {}
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
	bool m_bBuildAddon;
	CObj* m_pAddOn;
};
