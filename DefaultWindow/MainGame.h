#pragma once
#include "Obj.h"
#include "Player.h"
#include "Monster.h"

class MainGame
{
public:
	MainGame();
	~MainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;

	// 리스트 배열
	list<Obj*> m_ObjList[OBJ_END];

	ULONGLONG m_dwTime;
	int m_iFPS;
	TCHAR m_szFPS[16];
};
