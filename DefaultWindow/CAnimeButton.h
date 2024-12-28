#pragma once
#include "CObj.h"
class CAnimeButton : public CObj
{
public:
	CAnimeButton():m_bRender(false) {}
	~CAnimeButton() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Move_Frame() override;

private:
	bool m_bRender;
};

