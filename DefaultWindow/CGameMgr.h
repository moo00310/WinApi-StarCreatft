#pragma once
class CGameMgr
{
public:
	CGameMgr():m_iMineal(0), m_iGas(0), m_iPop(0) {}
	~CGameMgr() { Release(); }

	void		Initialize();
	void 		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	static CGameMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameMgr;

		return m_pInstance;
	}

	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	int m_iMineal;
	int m_iGas;
	int m_iPop;

	static CGameMgr* m_pInstance;
};

