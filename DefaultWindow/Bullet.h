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

private:
	WASD m_wasd = END;
};

