#pragma once
#include "Obj.h"

class Bullet : public Obj
{
public:
	enum WASD
	{
		W =1,
		A,
		S,
		D,
		END
	};
public:
	Bullet(WASD _wasd);
	virtual ~Bullet();

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	bool isOut()
	{
		return this->GetInfo().fX <= 100 || this->GetInfo().fY <= 100
			|| this->GetInfo().fX > WINCX - 100 || this->GetInfo().fY > WINCY - 100;
	}

private:
	WASD m_wasd = END;
};

