#pragma once
#include "CBuild.h"
class CAddon : public CBuild
{
public:
	CAddon(): isDoon(false) {}
	~CAddon() {}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput() override;
	void Change_Motion() override;

public:
	bool IsBuild() { return isDoon; }
private:
	bool isDoon;
};

