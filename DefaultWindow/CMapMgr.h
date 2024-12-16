#pragma once
class CMapMgr
{
private:
	CMapMgr();
	~CMapMgr();

public:
	void Initialize();

public:
	static CMapMgr* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CMapMgr();
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CMapMgr* m_pInstance;
	int map[75][75] = {};
};

