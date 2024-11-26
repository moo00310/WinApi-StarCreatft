#pragma once

#define WINCX  800
#define WINCY  600

#define PI 3.141592f
#define PURE = 0

extern HWND g_hWnd;

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