#include "pch.h"
#include "CBuild.h"
#include "CObjMgr.h"
#include "CMarine.h"
#include "CAbstractFactory.h"

void CBuild::SpawnUint()
{
	if (m_eCurState_Build == BS_MAKE ||
		m_eCurState_Build == BS_TEMP) return;

	if (!m_queSpawn.empty())
	{
		m_eCurState_Build = BS_RUN;

		OBJ_TYPE tpye = m_queSpawn.front();
		m_MaxSpwanTime = get<3>(ObjCost.at(tpye));

		if (m_MaxSpwanTime < m_iSpwanTime)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(450, 450));
			m_queSpawn.pop();
			m_iSpwanTime = 0;
		}
		m_iSpwanTime++;

	}
	else
	{
		m_eCurState_Build = BS_IDLE;
	}
}