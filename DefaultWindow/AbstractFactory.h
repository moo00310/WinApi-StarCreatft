#pragma once
#include "Obj.h"
template <typename T>
class AbstractFactory
{
public:
	AbstractFactory() {}
	~AbstractFactory() {}

public:
	static Obj* Create()
	{
		Obj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static Obj* Create(float _X, float _Y, DIRECTION _DIR)
	{
		Obj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_X, _Y);
		pObj->SetDir(_DIR);
		return pObj;
	}
};

