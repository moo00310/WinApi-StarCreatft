#include "pch.h"
#include "CBuild.h"
#include "CObjMgr.h"
#include "CMarine.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"

void CBuild::Spawn_Uint_CoolDown()
{
	if (m_eCurState_Build == BS_MAKE ||
		m_eCurState_Build == BS_TEMP) return;

	if (!m_listSpawn.empty())
	{
		m_eCurState_Build = BS_RUN;

		OBJ_TYPE tpye = m_listSpawn.front();
		m_MaxSpwanTime = get<3>(ObjCost.at(tpye));

		if (m_MaxSpwanTime < m_iSpwanTime)
		{
			Spawn_Uint_Index(tpye);
			m_listSpawn.pop_front();
			m_iSpwanTime = 0;
			m_fProgress = 0.f;
		}
		m_iSpwanTime++;

		m_fProgress += (134.f / m_MaxSpwanTime);
		m_fProgress = min(m_fProgress, 134.f);
	}
	else
	{
		m_eCurState_Build = BS_IDLE;
	}
}

void CBuild::Spawn_Uint_Index(OBJ_TYPE _id)
{
	Pos temp = { m_tInfo.fY / 32 +2, m_tInfo.fX / 32 };

	switch (_id)
	{
	case OT_Scv:
		break;
	case OT_Marine:
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(temp));
		break;
	case OT_Medic:
		break;
	case OT_Ghost:
		break;
	case OT_Tank:
		break;
	case OT_Science_Vessel:
		break;
	case OT_Unit_End:
		break;
	case OT_END:
		break;
	default:
		break;
	}
}
