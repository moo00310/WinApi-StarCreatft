#include "pch.h"
#include "CBuild.h"
#include "CObjMgr.h"
#include "CMarine.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CMapMgr.h"
#include "CGameMgr.h"
#include "CScv.h"
#include "CMedic.h"
#include "CGhost.h"
#include "CTank.h"
#include "CCovertOps.h"
#include "CSoundMgr.h"

void CBuild::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iCurCount;

		if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
			m_tFrame.iCurCount = m_tFrame.iFrameStart;

		m_tFrame.dwTime = GetTickCount64();
	}
}

void CBuild::Spawn_Uint_CoolDown()
{
	if (m_eCurState_Build == BS_MAKE ||
		m_eCurState_Build == BS_TEMP ||
		m_eCurState_Build == BS_LINK) return;

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
	Pos temp = { (int)m_tInfo.fY / 32 +2,(int)m_tInfo.fX / 32 };

	switch (_id)
	{
	case OT_Scv:
		CSoundMgr::Get_Instance()->PlaySFX(L"tscrdy00.wav", 0.8f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CScv>::Create(temp));
		break;
	case OT_Marine:
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMarine>::Create(temp));
		break;
	case OT_Medic:
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CMedic>::Create(temp));
		break;
	case OT_Ghost:
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CGhost>::Create(temp));
		break;
	case OT_Tank:
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CTank>::Create(temp));
		break;
	case OT_Science_Vessel:
		break;
	case OT_Unit_End:
		break;

	case OT_Marine_SightUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Marine_SightUp, true);
		break;
	case OT_Marine_Streampack:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Marine_Streampack, true);
		break;
	case OT_Medic_Magic1:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Medic_Magic1, true);
		break;
	case OT_Medic_Magic2:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Medic_Magic2, true);
		break;
	case OT_Medic_Mana:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Medic_Mana, true);
		break;
	case OT_Vulture_Speed:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Vulture_Speed, true);
		break;
	case OT_Vulture_mine:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Vulture_mine, true);
		break;
	case OT_Tank_SiegeMod:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Tank_SiegeMod, true);
		break;
	case OT_Gholiat_SightUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Gholiat_SightUp, true);
		break;
	case OT_Mecha_AtkUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Mecha_AtkUp, true);
		break;
	case OT_Sky_AtkUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Sky_AtkUp, true);
		break;
	case OT_Mecha_DefUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Mecha_DefUp, true);
		break;
	case OT_Sky_DefUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Sky_DefUp, true);
		break;
	case OT_Wirse_Cloak:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Wirse_Cloak, true);
		break;
	case OT_Wires_Mana:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Wires_Mana, true);
		break;
	case OT_Vessle_Emp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Vessle_Emp, true);
		break;
	case OT_Vessle_Irradiate:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Vessle_Irradiate, true);
		break;
	case OT_Vessle_Mana:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Vessle_Mana, true);
		break;
	case OT_Ghost_LockDown:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Ghost_LockDown, true);
		break;
	case OT_Ghost_Cloak:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Ghost_Cloak, true);
		break;
	case OT_Ghost_SightUp:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Ghost_SightUp, true);
		break;
	case OT_Ghost_Mana:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Ghost_Mana, true);
		break;
	case OT_Cmd_Nuke:
		CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Cmd_Nuke, true);
		break;
	case OT_END:
		break;
	default:
		break;
	}
	
}

void CBuild::Block_Map()
{
	Pos pos = { (m_tRect.top) / 32,(m_tRect.left) / 32 };

	for (int i = 0; i < m_tInfo.fCY / 32; i++)
	{
		for (int j = 0; j < m_tInfo.fCX / 32; j++)
		{
			Pos temp = { i,j };
			CMapMgr::Get_Instance()->SetTileType(pos + temp, 2);
		}
	}
}

void CBuild::UnBlock_Map()
{
	Pos pos = { (m_tRect.top) / 32,(m_tRect.left) / 32 };

	for (int i = 0; i < m_tInfo.fCY / 32; i++)
	{
		for (int j = 0; j < m_tInfo.fCX / 32; j++)
		{
			Pos temp = { i,j };
			CMapMgr::Get_Instance()->SetTileType(pos + temp, 0);
		}
	}
}

bool CBuild::ChekList_OBJ(OBJ_TYPE _id)
{
	for (auto ID : m_listSpawn)
	{
		if (ID == _id)
			return true;
	}

	return false;
}



