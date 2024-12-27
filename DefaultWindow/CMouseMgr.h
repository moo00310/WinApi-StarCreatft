#pragma once
#include "CObj.h"
#include "CGameMouse.h"

class CMouseMgr
{
public:
	CMouseMgr() :m_pMouse(nullptr) {}
	~CMouseMgr() { Release(); }
	
	void 		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	void Add_Mouse(CObj* _mouse);
	CGameMouse* Get_Mouse()
	{ 
		CGameMouse* temp = static_cast<CGameMouse*>(m_pMouse);
		return temp;
	}

public:
	static CMouseMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CMouseMgr;

		return m_pInstance;
	}

	static void		DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	CObj* m_pMouse;

	static CMouseMgr* m_pInstance;
};

