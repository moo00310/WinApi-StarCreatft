#pragma once

#include "CObj.h"
#include "Define.h"

class CCollisionMgr
{
public:
	static void	Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src);
	static bool	Check_Circle(CObj* _Dst, CObj* _Src);

	static void	Collision_RectEx(CObj* _Dst, list<CObj*> _Src);
	static bool	Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY);
	///////////////////////////////////////////////////////////////////////////

	static CObj* Collision_RangeChack(CObj* _pPlayer, list<CObj*> _pMonster, float _dis);
	static bool Collision_Range_Bool(CObj* _pPlayer, CObj* _unit, float _dis);
	static bool Collision_RangeChack_bool(CObj* _pPlayer, list<CObj*> _Src, float _dis);
	static CObj* Collision_Rect_Mouse(RECT rect, list<CObj*> _Unit, list<CObj*> _Build, list<CObj*> _EUnit, list<CObj*> _EBuild);
	static void Collision_Rect_Mouse_RECT(RECT rect, list<CObj*> _Src, list<CObj*>* select);

	static CObj* Collision_RangeChack_Heal(CObj* _pPlayer, list<CObj*> _unit, float _dis);
	static CObj* Collision_RangeChack_Attack(CObj* _pPlayer, list<CObj*> _unit, list<CObj*>_build, float _dis);

	static Pos Collision_RangePos(CObj* _pPlayer, CObj* _unit, float _dis);

	static void Collision_Explosion(CObj* _pEx, list<CObj*> _unit, list<CObj*> _build);
	static bool Collision_Unit_body(CObj* _my, list<CObj*> _unit);
};

