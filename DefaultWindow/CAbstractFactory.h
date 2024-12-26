#pragma once

#include "CObj.h"
#include "CUI.h"
#include "CMemoryPoolMgr.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {};

public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _fX, float _fY, float _fAngle = 0.f)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		
		if(0.f != _fAngle)
			pObj->Set_Angle(_fAngle);

		return pObj;
	}

	static CObj* Create(Pos pos)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(pos.x * 32.f, pos.y * 32.f);
		pObj->Initialize();

		return pObj;
	}


	static CUI* CreateUI()
	{
		CUI* pUi = new T;
		pUi->Initialize();
		return pUi;
	}

	static CObj* CreateFX(float _fX, float _fY)
	{
		// 메모리 풀에서 메모리 할당
		void* temp = CMemoryPoolMgr::Get_Instance()->allocate();

		CObj* pObj = new (temp) T();
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		return pObj;
	}
};

