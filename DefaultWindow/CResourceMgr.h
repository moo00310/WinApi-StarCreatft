#pragma once
#include "CResource.h"
class CResourceMgr
{
private:
	CResourceMgr() {}
	~CResourceMgr() {}
public:
	void Update();
	void Render(HDC hDC);
	void Release();
public:
	void		Save_Tile();
	void		Load_Tile();
public:
	static CResourceMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CResourceMgr;

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

