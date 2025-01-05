#include "pch.h"
#include "CCollisionMgr.h"
#include "CGameMouse.h"
#include "CUnit.h"

void CCollisionMgr::Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Circle(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Circle(CObj* _Dst, CObj* _Src)
{
	float fRadius = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;

	float fWidth  = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fHeight = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	return fRadius >= fDiagonal;
}

void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				// 상 하 충돌
				if (fX > fY)
				{
					// 상 충돌
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
					{
						Dst->Set_PosY(-fY);
					}
					// 하 충돌
					else
					{
						Dst->Set_PosY(+fY);
					}
				}
				
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{
						Dst->Set_PosX(-fX);
					}
					// 우 충돌
					else
					{
						Dst->Set_PosX(fX);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY)
{

	float		fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float		fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float		fRadiusX = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;
	float		fRadiusY = (_Dst->Get_Info().fCY + _Src->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fX) && (fRadiusY >= fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;
		return true;
	}

	return false;
}

/// /////////////////////////////////////////////////////////////////////////////////

CObj* CCollisionMgr::Collision_RangeChack(CObj* _pPlayer, list<CObj*> _pMonster, float _dis)
{
	for (auto monster : _pMonster)
	{
		float fWidth = fabsf(monster->Get_Scroll_Info().fX - _pPlayer->Get_Scroll_Info().fX);
		float fHeight = fabsf(monster->Get_Scroll_Info().fY - _pPlayer->Get_Scroll_Info().fY);

		float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (fDistance <= _dis)
			return monster;
	}

	return nullptr;
}

bool CCollisionMgr::Collision_Range_Bool(CObj* _pPlayer, CObj* _unit, float _dis)
{

	float fWidth = fabsf(_unit->Get_Info().fX - _pPlayer->Get_Info().fX);
	float fHeight = fabsf(_unit->Get_Info().fY - _pPlayer->Get_Info().fY);

	float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fDistance <= _dis)
		return true;
	else
		return false;
}

bool CCollisionMgr::Collision_RangeChack_bool(CObj* _pPlayer, list<CObj*> _Src, float _dis)
{
	for (auto& Src : _Src)
	{
		if (static_cast<CUnit*>(Src)->GetAinmeState() != STATE_MOVE) continue;

		float dx = _pPlayer->Get_Scroll_Info().fX - Src->Get_Scroll_Info().fX;
		float dy = _pPlayer->Get_Scroll_Info().fY - Src->Get_Scroll_Info().fY;
		float distance = sqrtf(dx * dx + dy * dy);
		 
		if (distance > _dis || distance == 0)
			continue;

		float x1(0.f), y1(0.f), x2(0.f), y2(0.f);
		DIRECTION dir = _pPlayer->Get_Direction();
		fPOINT point = Nomalization(MoveFront[dir]);

		x1 = _pPlayer->Get_Scroll_Info().fX;
		y1 = _pPlayer->Get_Scroll_Info().fY;
		x2 = x1 + _dis * point.x;
		y2 = y1 + _dis * point.y;
		
		// 외적(cross product) 계산
		float crossProduct = (Src->Get_Scroll_Info().fY - y1) * (x2 - x1) - (Src->Get_Scroll_Info().fX - x1) * (y2 - y1);

		// 외적 오차 허용
		if (fabsf(crossProduct) > 1200.f)
		{
			continue;
		}

		// 선분 범위 안에 있는지 확인
		if (Src->Get_Scroll_Info().fX >= min(x1, x2) && Src->Get_Scroll_Info().fX <= max(x1, x2) &&
			Src->Get_Scroll_Info().fY >= min(y1, y2) && Src->Get_Scroll_Info().fY <= max(y1, y2))
		{
			return true; // 충돌 발생
		}
	}
	return false; // 충돌 없음
}


CObj* CCollisionMgr::Collision_Rect_Mouse(RECT rect, list<CObj*> _Unit, list<CObj*> _Build , list<CObj*> _EUnit, list<CObj*> _EBuild)
{
	RECT rc{};

	for (auto& _unit : _Unit)
	{
		if (IntersectRect(&rc, &rect, _unit->Get_Scroll_Rect()))
		{
			return _unit;
		}
	}

	for (auto& _build : _Build)
	{
		if (IntersectRect(&rc, &rect, _build->Get_Scroll_Rect()))
		{
			return _build;
		}
	}

	for (auto& _Eunit : _EUnit)
	{
		if (IntersectRect(&rc, &rect, _Eunit->Get_Scroll_Rect()))
		{
			return _Eunit;
		}
	}

	for (auto& _Ebuild : _EBuild)
	{
		if (IntersectRect(&rc, &rect, _Ebuild->Get_Scroll_Rect()))
		{
			return _Ebuild;
		}
	}

	return nullptr;
	
}


void CCollisionMgr::Collision_Rect_Mouse_RECT(RECT rect, list<CObj*> _Src, list<CObj*>* select)
{
	RECT rc{};
	int count = 0;
	for (auto& Src : _Src)
	{
		if (IntersectRect(&rc, &rect, Src->Get_Scroll_Rect()))
		{
			if (count >= 12) return;
			select->push_back(Src);
			Src->Set_Select(true);
			count++;
		}
	}
}

CObj* CCollisionMgr::Collision_RangeChack_Heal(CObj* _pPlayer, list<CObj*> _unit, float _dis)
{
	for (auto unit : _unit)
	{
		if(OT_Tank == unit->Get_ObjID()|| OT_SiegeTank == unit->Get_ObjID()) continue;
		if((unit->Get_Stat()->m_iHp - unit->Get_Stat()->m_iMaxHp) >= 0.f) continue;

		float fWidth = fabsf(unit->Get_Scroll_Info().fX - _pPlayer->Get_Scroll_Info().fX);
		float fHeight = fabsf(unit->Get_Scroll_Info().fY - _pPlayer->Get_Scroll_Info().fY);
		float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (fDistance <= _dis)
			return unit;
	}

	return nullptr;
}

Pos CCollisionMgr::Collision_RangePos(CObj* _pPlayer, CObj* _unit, float _dis)
{
	float fWidth = _unit->Get_Info().fX - _pPlayer->Get_Info().fX;
	float fHeight = _unit->Get_Info().fY - _pPlayer->Get_Info().fY;

	float distance = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float x = _unit->Get_Info().fX + (_dis * -1 * fWidth / distance);
	float y = _unit->Get_Info().fY + (_dis * -1 * fHeight / distance);

	Pos pos{(int)(y / 32), (int)(x / 32) };

	return pos;
}

void CCollisionMgr::Collision_Explosion(CObj* _pEx, list<CObj*> _unit, list<CObj*> _build)
{
	RECT rc{};

	for (auto& unit : _unit)
	{
		if (IntersectRect(&rc, _pEx->Get_Rect(), unit->Get_Rect()))
		{
			unit->Add_Stat_hp(-50);
		}
	}

	for (auto& build : _build)
	{
		if (IntersectRect(&rc, _pEx->Get_Rect(), build->Get_Rect()))
		{
			build->Add_Stat_hp(-50);
		}
	}
	
	return;
}

CObj* CCollisionMgr::Collision_RangeChack_Attack(CObj* _pPlayer, list<CObj*> _unit, list<CObj*>_build, float _dis)
{

	for (auto unit : _unit)
	{
		float fWidth = fabsf(unit->Get_Scroll_Info().fX - _pPlayer->Get_Scroll_Info().fX);
		float fHeight = fabsf(unit->Get_Scroll_Info().fY - _pPlayer->Get_Scroll_Info().fY);
		float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (_pPlayer->Get_ObjID() == OT_SiegeTank && fDistance < 80.f)
			continue;

		if (fDistance <= _dis)
			return unit;
	}

	if(_pPlayer->Get_ObjID() == OT_Medic) return nullptr;

	for (auto unit : _build)
	{
		float fWidth = fabsf(unit->Get_Scroll_Info().fX - _pPlayer->Get_Scroll_Info().fX);
		float fHeight = fabsf(unit->Get_Scroll_Info().fY - _pPlayer->Get_Scroll_Info().fY);
		float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (fDistance <= _dis)
			return unit;
	}


	return nullptr;
}
