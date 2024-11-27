#include "pch.h"
#include "CollisionMgr.h"

void CollisionMgr::Collision_Rect(list<Obj*> _Dst, list<Obj*> _Src)
{
	RECT ct{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&ct, Dst->GetRect(), Src->GetRect()))
			{
				Dst->SetDead();
				Src->SetDead();
			}
		}
	}
}

void CollisionMgr::Collision_Circle(list<Obj*> _Temp, list<Obj*> _Src)
{
	for (auto& Temp : _Temp)
	{
		float radius1 = Temp->GetInfo().fCX * 0.5f;
		for (auto& Src : _Src)
		{
			float radius2 = Src->GetInfo().fCX * 0.5f;
			float distance = sqrt(pow(Src->GetInfo().fX - Temp->GetInfo().fX, 2)
				+ pow(Src->GetInfo().fY - Temp->GetInfo().fY, 2));
			if (distance <= radius1 + radius2)
			{
				Temp->SetDead();
				Src->SetDead();
			}

		}
	}
}
