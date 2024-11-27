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

void CollisionMgr::Collision_Circle(list<Obj*> _Circle, list<Obj*> _Src)
{
	for (auto& Circle : _Circle)
	{
		float radius = Circle->GetInfo().fCX * 0.5f;
		for (auto& Src : _Src)
		{
			float distance(0);
			distance = sqrt(pow(Src->GetInfo().fX - Circle->GetInfo().fX, 2) 
				+ pow(Src->GetInfo().fY - Circle->GetInfo().fY, 2)); 
			if (distance <= radius)
			{
				Circle->SetDead();
				Src->SetDead();
			}

		}
	}
}
