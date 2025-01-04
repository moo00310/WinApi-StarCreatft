#pragma once

#include "CObj.h"
#include "CBuild.h"
#include "CUI.h"
#include "CMemoryPoolMgr.h"
#include "CResource.h"

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

	static CObj* Create(float _fX, float _fY)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
	
		return pObj;
	}

	static CObj* Create(Pos pos)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(pos.x * 32.f, pos.y * 32.f);
		pObj->Initialize();

		return pObj;
	}
	
	static CObj* CreateBuild(float _fX, float _fY)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		static_cast<CBuild*>(pObj)->Set_State(BS_IDLE);

		return pObj;
	}

	static CObj* CreateButton(float _fX, float _fY, const TCHAR* _name)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_ImgKey(_name);
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

	static CResource* CreateResource(float _fX, float _fY, int num)
	{
		CResource* pResource = new T;
		pResource->SetPos(_fX, _fY);
		pResource->SetOption(num);

		return pResource;
	}
	
};

