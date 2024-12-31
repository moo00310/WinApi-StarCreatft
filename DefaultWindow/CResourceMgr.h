#pragma once
#include "CResource.h"
class CResourceMgr
{
private:
	CResourceMgr() {}
	~CResourceMgr() { Release(); }
public:
	void Update();
	void Render(HDC hDC);
	void Release();
public:
	void		Save_Resorce();
	void		Load_Resorce();
	void		Push_Resource(CResource* resoure) { m_vecResource.push_back(resoure); }
	vector<CResource*>* Get_Vec() { return &m_vecResource; }

public:
	static CResourceMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CResourceMgr();

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CResourceMgr* m_pInstance;
	vector<CResource*> m_vecResource;
};

