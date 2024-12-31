#pragma once
#include "CResource.h"
class CMineral : public CResource
{
public:
	CMineral() {}
	~CMineral() {}
public:
	void Initailize() override;
	void Update() override;
	void Render(HDC hDC) override;
};

