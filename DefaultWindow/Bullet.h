#pragma once
#include "Obj.h"

class Bullet : public Obj
{
public:

public:
	Bullet();
	virtual ~Bullet();

public:
	void Initialize() override;
	int	 Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	bool isOut()
	{
		return this->GetInfo().fX <= 100 || this->GetInfo().fY <= 100
			|| this->GetInfo().fX > WINCX - 100 || this->GetInfo().fY > WINCY - 100;
	}

private:
};

