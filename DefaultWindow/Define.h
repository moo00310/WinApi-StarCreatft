#pragma once

#define WINCX  800
#define WINCY  600

#define PI 3.141592f
#define PURE = 0

#define OBJ_NOEVENT		0
#define OBJ_DEAD		1

extern HWND g_hWnd;

enum OBJID { OBJ_PLAYER, OBJ_BULLET, OBJ_MONSTER, OBJ_MOUSE, OBJ_BUTTON, OBJ_END };
enum DIRECTION {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_END };

typedef struct Info
{
	float		fX, fY;
	float		fCX, fCY;	// 가로, 세로 길이

}INFO;

template <typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}