#pragma once

#include "Define.h"
#include "CObj.h"
#include "CUnit.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;
	CObj operator =(CObj& rObj) = delete;
	~CObjMgr();

public:
	CObj* Get_Player()		{ return m_ObjList[OBJ_PLAYER].front(); }
	list<CObj*>* Get_MonsterList() { return &m_ObjList[OBJ_MONSTER]; }
	list<CObj*>* Get_ObjList(OBJID _id) { return &m_ObjList[_id]; }
	CObj* Get_Target(OBJID eID, CObj* pDst);
	CObj* Get_PickObj() { return m_PickObj; }

public:
	void Add_SelectList(CObj*& pObj);
	list<CObj*>* Get_Select_List() { return &m_SelectList; }
	//void DeleteObject(CObj* pObj);

public:
	void		Add_Object(OBJID eID, CObj* pObj);
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Delete_ID(OBJID eID);

public:
	static CObjMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CObjMgr;

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

private:
	//set<CObj*> m_CentralList;
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_RenderList[RENDER_END];

	list<CObj*>	m_SelectList;
	CObj* m_PickObj;

	static CObjMgr* m_pInstance;


};

