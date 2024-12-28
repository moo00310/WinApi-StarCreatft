#pragma once
#include "Define.h"

class CMapMgr
{
private:
	CMapMgr() : m_dwTime(GetTickCount64()) {}
	~CMapMgr() {}

public:
	void Initialize_Map();
	void Render(HDC hdc);

	//TODO
	// 인덱스를 사용해서 이동 가능 불가능 변경(건물, 광물)

	// 맵의 정보를 Unit에게 전달
	vector<vector<int>>* GetMap() { return &m_Map; }

	int GetTileType(Pos pos) 
	{ 
		if (pos.x < 0 || pos.x >127 || pos.y < 0 || pos.y >127) return 999;
		return m_Map[pos.y][pos.x]; 
	}
	void SetTileType(Pos pos, int _Option)
	{ 
		if (pos.x < 0 || pos.x >127 || pos.y < 0 || pos.y >127) return;
		m_Map[pos.y][pos.x] = _Option; 
	}


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
	ULONGLONG m_dwTime;
	vector<vector<int>> m_Map = vector<vector<int>>(128, vector<int>(128, 0));
};

