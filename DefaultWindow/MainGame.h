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
	void Render();
	void Release();

private:
	HDC m_hDC; // 그림을 그리는데 필요한 변수

	Obj* m_pPlayer;
	list<Obj*> m_BulletList;
	Obj* m_pMonster;

};

