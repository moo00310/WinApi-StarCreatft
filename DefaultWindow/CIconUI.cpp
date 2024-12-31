#include "pch.h"
#include "CIconUI.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CUnit.h"
#include "CBuild.h"
#include "CGameMgr.h"
#include "CKeyMgr.h"
#include "CScv.h"
#include "CFactory.h"

CIconUI::CIconUI() : IconCX(0), IconCY(0), m_buildState(BS_END), m_UnitState(STATE_END), m_InputState(IP_END), m_listSpawn(nullptr)
{
}

CIconUI::~CIconUI()
{
}

void CIconUI::Initialize()
{
    m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
	IconCX = 44;
	IconCY = 44;
}

int CIconUI::Update()
{
	if (m_pUintlist->empty())
	{
		m_eCurState = OT_END;
		m_bRender = false;
		return 0;
	}

	if (m_pUintlist->size() == 1)
	{
		// 해당 오브젝트의 아이디를 가져와야하고, 그 오브젝트의 상태도 가져 와야함 

		m_eCurState = m_pUintlist->front()->Get_ObjID();
		m_bRender = true;
		Change_Button();
	}

	if (m_pUintlist->size() > 1)
	{
		//여려 마리 일때 어캐할건지
	}

	return 0;
}

void CIconUI::Late_Update()
{
}

void CIconUI::Render(HDC hdc)
{
	if (!m_bRender) return;

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonIcon");

	// 1번 버튼
	{
		BitBlt(hdc,
			633, 449, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[0].first,
			IconCY * m_Button_Icon[0].second,
			SRCCOPY);
	}

	// 2번 버튼
	{
		BitBlt(hdc,
			690, 449, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[1].first,
			IconCY * m_Button_Icon[1].second,
			SRCCOPY);
	}

	// 3번 버튼
	{
		BitBlt(hdc,
			748, 449, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[2].first,
			IconCY * m_Button_Icon[2].second,
			SRCCOPY);
	}

	// 4번 버튼
	{
		BitBlt(hdc,
			633, 499, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[3].first,
			IconCY * m_Button_Icon[3].second,
			SRCCOPY);
	}
	
	// 5번 버튼
	{
		BitBlt(hdc,
			690, 499, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[4].first,
			IconCY * m_Button_Icon[4].second,
			SRCCOPY);
	}

	// 6번 버튼
	{
		BitBlt(hdc,
			748, 499, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[5].first,
			IconCY * m_Button_Icon[5].second,
			SRCCOPY);
	}

	// 7번 버튼
	{
		BitBlt(hdc,
			633, 549, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[6].first,
			IconCY * m_Button_Icon[6].second,
			SRCCOPY);
	}

	// 8번 버튼
	{
		BitBlt(hdc,
			690, 549, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[7].first,
			IconCY * m_Button_Icon[7].second,
			SRCCOPY);
	}

	// 9번 버튼
	{
		BitBlt(hdc,
			748, 549, 44, 41,
			hMemDC,
			IconCX * m_Button_Icon[8].first,
			IconCY * m_Button_Icon[8].second,
			SRCCOPY);
	}

}

void CIconUI::Release()
{
}

void CIconUI::Change_Button()
{
	if (m_eCurState < OT_Unit_End)
	{
		m_UnitState = static_cast<CUnit*>(m_pUintlist->front())->GetAinmeState();
		m_InputState = static_cast<CUnit*>(m_pUintlist->front())->GetInput();
	}
	else if (m_eCurState < OT_Build_End)
	{
		m_buildState = static_cast<CBuild*>(m_pUintlist->front())->Get_State();
		m_listSpawn = static_cast<CBuild*>(m_pUintlist->front())->Get_SpawnList();
	}
	else
		return;

	switch (m_eCurState)
	{
	case OT_Scv:
		SCV();
		break;
	case OT_Marine:
		Marine();
		break;
	case OT_Medic:
		Medic();
		break;
	case OT_Ghost:
		Ghost();
		break;
	case OT_Tank:
		Tank();
		break;
	case OT_SiegeTank:
		SiegeTank();
		break;
	case OT_Science_Vessel:
		break;
	case OT_Unit_End:
		break;
	case OT_Commend:
		CommedCenter();
		break;
	case OT_Suffly:
		Suffly();
		break;
	case OT_Refinery:
		Refinery();
		break;
	case OT_Barrck:
		Barrack();
		break;
	case OT_Academy:
		Archerdemy();
		break;
	case OT_Factory:
		Factory();
		break;
	case OT_Addon:
		FactoryAddon();
		break;
	case OT_Armory:
		Armory();
		break;
	case OT_Starport:
		Starport();
		break;
	case OT_StarportAddOn:
		StarportAddon();
		break;
	case OT_ScienceFacility:
		Siencefacilly();
		break;
	case OT_ScienceSecret:
		CovertOps();
		break;
	case OT_CmdNuke:
		cmdNuke();
		break;
	case OT_Build_End:
		break;
	case OT_END:
		break;
	default:
		break;
	}
}

void CIconUI::SCV()
{
	if (static_cast<CScv*>(m_pUintlist->front())->GetBulid())
	{
		m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Commend;

		m_Button_Icon[1].first = 0;
		m_Button_Icon[1].second = IC_Suffly;

		m_Button_Icon[2].first = 0;
		m_Button_Icon[2].second = IC_Refinery;

		m_Button_Icon[3].first = 0;
		m_Button_Icon[3].second = IC_Barrack;

		m_Button_Icon[4].first = 4;
		m_Button_Icon[4].second = IC_Enginer;

		m_Button_Icon[5].first = 4;
		m_Button_Icon[5].second = IC_Terret;

		if(CGameMgr::Get_Instance()->GetTechCount(TECH_Braack) > 0)
			m_Button_Icon[6].first = 0;
		else
			m_Button_Icon[6].first = 4;
		m_Button_Icon[6].second = IC_Archerdemy;


		m_Button_Icon[7].first = 4;
		m_Button_Icon[7].second = IC_Bungker;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else if (static_cast<CScv*>(m_pUintlist->front())->GetAdBuild())
	{
		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Braack) > 0)
			m_Button_Icon[0].first = 0;
		else
			m_Button_Icon[0].first = 4;
		m_Button_Icon[0].second = IC_Factory;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Factory) > 0)
			m_Button_Icon[1].first = 0;
		else
			m_Button_Icon[1].first = 4;
		m_Button_Icon[1].second = IC_Starport;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Starport) > 0)
			m_Button_Icon[2].first = 0;
		else
			m_Button_Icon[2].first = 4;
		m_Button_Icon[2].second = IC_Siencefacilly;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Factory) > 0)
			m_Button_Icon[3].first = 0;
		else
			m_Button_Icon[3].first = 4;
		m_Button_Icon[3].second = IC_Armory;

		m_Button_Icon[4].first = 999;
		m_Button_Icon[4].second = 999;

		m_Button_Icon[5].first = 999;
		m_Button_Icon[5].second = 999;

		m_Button_Icon[6].first = 999;
		m_Button_Icon[6].second = 999;

		m_Button_Icon[7].first = 999;
		m_Button_Icon[7].second = 999;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if(m_UnitState == STATE_MOVE)
			m_Button_Icon[0].first = 1;
		else
			m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Move;

		if (m_UnitState == STATE_IDLE)
			m_Button_Icon[1].first = 1;
		else
			m_Button_Icon[1].first = 0;
		m_Button_Icon[1].second = IC_Stop;

		if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
			m_Button_Icon[2].first = 1;
		else
			m_Button_Icon[2].first = 0;
		m_Button_Icon[2].second = IC_Attack;

		if (m_InputState == IP_BUILD)
		{
			m_Button_Icon[3].first = 1;
			m_Button_Icon[4].first = 99;
			m_Button_Icon[6].first = 99;
			m_Button_Icon[7].first = 99;
		}
		else
		{
			m_Button_Icon[3].first = 0;
			m_Button_Icon[4].first = 0;
			m_Button_Icon[6].first = 0;
			m_Button_Icon[7].first = 0;
		}
			
		m_Button_Icon[3].second = IC_Refair;
		m_Button_Icon[4].second = IC_Gater;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].second = IC_Build;
		m_Button_Icon[7].second = IC_AdBUild;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}

}

void CIconUI::Marine()
{
	if (m_UnitState == STATE_MOVE)
		m_Button_Icon[0].first = 1;
	else
		m_Button_Icon[0].first = 0;
	m_Button_Icon[0].second = IC_Move;

	if (m_UnitState == STATE_IDLE)
		m_Button_Icon[1].first = 1;
	else
		m_Button_Icon[1].first = 0;
	m_Button_Icon[1].second = IC_Stop;

	if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
		m_Button_Icon[2].first = 1;
	else
		m_Button_Icon[2].first = 0;
	m_Button_Icon[2].second = IC_Attack;

	m_Button_Icon[3].first = 0;
	m_Button_Icon[3].second = IC_Patrol;

	if(m_InputState == IP_HOLD)
		m_Button_Icon[4].first = 1;
	else
		m_Button_Icon[4].first = 0;
	m_Button_Icon[4].second = IC_Hold;


	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 99;
	m_Button_Icon[7].second = 99;

	if (CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_Streampack))
	{
		if (CKeyMgr::Get_Instance()->GetKeyState('T'))
			m_Button_Icon[8].first = 1;
		else
			m_Button_Icon[8].first = 0;
	}
	else
		m_Button_Icon[8].first = 4;
	m_Button_Icon[8].second = IC_MarineSteamPack;

}

void CIconUI::Medic()
{
	if (m_UnitState == STATE_MOVE)
		m_Button_Icon[0].first = 1;
	else
		m_Button_Icon[0].first = 0;
	m_Button_Icon[0].second = IC_Move;

	if (m_UnitState == STATE_IDLE)
		m_Button_Icon[1].first = 1;
	else
		m_Button_Icon[1].first = 0;
	m_Button_Icon[1].second = IC_Stop;

	if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
		m_Button_Icon[2].first = 1;
	else
		m_Button_Icon[2].first = 0;
	m_Button_Icon[2].second = IC_Attack;

	m_Button_Icon[3].first = 0;
	m_Button_Icon[3].second = IC_Patrol;

	if (m_InputState == IP_HOLD)
		m_Button_Icon[4].first = 1;
	else
		m_Button_Icon[4].first = 0;
	m_Button_Icon[4].second = IC_Hold;


	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 0;
	m_Button_Icon[7].second = IC_MedicMegic_1;

	m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_MedicMegic_2;
}

void CIconUI::Ghost()
{
	if (m_UnitState == STATE_MOVE)
		m_Button_Icon[0].first = 1;
	else
		m_Button_Icon[0].first = 0;
	m_Button_Icon[0].second = IC_Move;

	if (m_UnitState == STATE_IDLE)
		m_Button_Icon[1].first = 1;
	else
		m_Button_Icon[1].first = 0;
	m_Button_Icon[1].second = IC_Stop;

	if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
		m_Button_Icon[2].first = 1;
	else
		m_Button_Icon[2].first = 0;
	m_Button_Icon[2].second = IC_Attack;

	m_Button_Icon[3].first = 0;
	m_Button_Icon[3].second = IC_Patrol;

	if (m_InputState == IP_HOLD)
		m_Button_Icon[4].first = 1;
	else
		m_Button_Icon[4].first = 0;
	m_Button_Icon[4].second = IC_Hold;


	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 0;
	m_Button_Icon[6].second = IC_Ghost_Cloak;

	m_Button_Icon[7].first = 0;
	m_Button_Icon[7].second = IC_Ghost_LockDown;

	m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_Ghost_Nuke;
}

void CIconUI::Tank()
{
	if (m_UnitState == STATE_MOVE)
		m_Button_Icon[0].first = 1;
	else
		m_Button_Icon[0].first = 0;
	m_Button_Icon[0].second = IC_Move;

	if (m_UnitState == STATE_IDLE)
		m_Button_Icon[1].first = 1;
	else
		m_Button_Icon[1].first = 0;
	m_Button_Icon[1].second = IC_Stop;

	if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
		m_Button_Icon[2].first = 1;
	else
		m_Button_Icon[2].first = 0;
	m_Button_Icon[2].second = IC_Attack;

	m_Button_Icon[3].first = 0;
	m_Button_Icon[3].second = IC_Patrol;

	if (m_InputState == IP_HOLD)
		m_Button_Icon[4].first = 1;
	else
		m_Button_Icon[4].first = 0;
	m_Button_Icon[4].second = IC_Hold;


	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 99;
	m_Button_Icon[7].second = 99;

	if (CKeyMgr::Get_Instance()->GetKeyState('E'))
		m_Button_Icon[8].first = 1;
	else
		m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_Siegemod;
}

void CIconUI::SiegeTank()
{
	if (m_UnitState == STATE_MOVE)
		m_Button_Icon[0].first = 1;
	else
		m_Button_Icon[0].first = 0;
	m_Button_Icon[0].second = IC_Move;

	if (m_UnitState == STATE_IDLE)
		m_Button_Icon[1].first = 1;
	else
		m_Button_Icon[1].first = 0;
	m_Button_Icon[1].second = IC_Stop;

	if (m_UnitState == STATE_ATTACK || m_UnitState == STATE_SHOOT)
		m_Button_Icon[2].first = 1;
	else
		m_Button_Icon[2].first = 0;
	m_Button_Icon[2].second = IC_Attack;

	m_Button_Icon[3].first = 0;
	m_Button_Icon[3].second = IC_Patrol;

	if (m_InputState == IP_HOLD)
		m_Button_Icon[4].first = 1;
	else
		m_Button_Icon[4].first = 0;
	m_Button_Icon[4].second = IC_Hold;


	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 99;
	m_Button_Icon[7].second = 99;

	if (CKeyMgr::Get_Instance()->GetKeyState('E'))
		m_Button_Icon[8].first = 1;
	else
		m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_Tankmod;
}

void CIconUI::CommedCenter()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (CKeyMgr::Get_Instance()->GetKeyState('S'))
			m_Button_Icon[0].first = 1;
		else
			m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Scv;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Academy) > 0)
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('C'))
				m_Button_Icon[6].first = 1;
			else
				m_Button_Icon[6].first = 0;
		}
		else
			m_Button_Icon[6].first = 4;
		m_Button_Icon[6].second = IC_Scan_Addon;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_CovertOps) > 0)
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('N'))
				m_Button_Icon[7].first = 1;
			else
				m_Button_Icon[7].first = 0;
		}
		else
			m_Button_Icon[7].first = 4;
		m_Button_Icon[7].second = IC_Nuke_Addon;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
	
}

void CIconUI::Suffly()
{
	m_Button_Icon[0].first = 99;
	m_Button_Icon[0].second = 99;

	m_Button_Icon[1].first = 99;
	m_Button_Icon[1].second = 99;

	m_Button_Icon[2].first = 99;
	m_Button_Icon[2].second = 99;

	m_Button_Icon[3].first = 99;
	m_Button_Icon[3].second = 99;

	m_Button_Icon[4].first = 99;
	m_Button_Icon[4].second = 99;

	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 99;
	m_Button_Icon[7].second = 99;

	m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_Cancel;
}

void CIconUI::Refinery()
{
	m_Button_Icon[0].first = 99;
	m_Button_Icon[0].second = 99;

	m_Button_Icon[1].first = 99;
	m_Button_Icon[1].second = 99;

	m_Button_Icon[2].first = 99;
	m_Button_Icon[2].second = 99;

	m_Button_Icon[3].first = 99;
	m_Button_Icon[3].second = 99;

	m_Button_Icon[4].first = 99;
	m_Button_Icon[4].second = 99;

	m_Button_Icon[5].first = 99;
	m_Button_Icon[5].second = 99;

	m_Button_Icon[6].first = 99;
	m_Button_Icon[6].second = 99;

	m_Button_Icon[7].first = 99;
	m_Button_Icon[7].second = 99;

	m_Button_Icon[8].first = 0;
	m_Button_Icon[8].second = IC_Cancel;
}

void CIconUI::Barrack()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (CKeyMgr::Get_Instance()->GetKeyState('A'))
			m_Button_Icon[0].first = 1;
		else
			m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Marine;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Academy) > 0)
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('F'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
		}
		else
			m_Button_Icon[1].first = 4;
		m_Button_Icon[1].second = IC_firebat;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_CovertOps) > 0)
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('G'))
				m_Button_Icon[2].first = 1;
			else
				m_Button_Icon[2].first = 0;
		}
		else
			m_Button_Icon[2].first = 4;
		m_Button_Icon[2].second = IC_Ghost;

		if (CGameMgr::Get_Instance()->GetTechCount(TECH_Academy) > 0)
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('E'))
				m_Button_Icon[3].first = 1;
			else
				m_Button_Icon[3].first = 0;
		}
		else
			m_Button_Icon[3].first = 4;
		m_Button_Icon[3].second = IC_Medic;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::Archerdemy()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{

		if (ChekList_OBJ(OT_Marine_SightUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_SightUp))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('E'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_MarineRichUp;
		}


		if (ChekList_OBJ(OT_Marine_Streampack) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Marine_Streampack))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('T'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_MarineSteamPack;
		}

		
		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		if (ChekList_OBJ(OT_Medic_Magic1) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Magic1))
		{
			m_Button_Icon[3].first = 99;
			m_Button_Icon[3].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('Z'))
				m_Button_Icon[3].first = 1;
			else
				m_Button_Icon[3].first = 0;
			m_Button_Icon[3].second = IC_MedicMegic_1;
		}


		if (ChekList_OBJ(OT_Medic_Magic2) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Magic2))
		{
			m_Button_Icon[4].first = 99;
			m_Button_Icon[4].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('X'))
				m_Button_Icon[4].first = 1;
			else
				m_Button_Icon[4].first = 0;
			m_Button_Icon[4].second = IC_MedicMegic_2;
		}

		if (ChekList_OBJ(OT_Medic_Mana) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Medic_Mana))
		{
			m_Button_Icon[5].first = 99;
			m_Button_Icon[5].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('C'))
				m_Button_Icon[5].first = 1;
			else
				m_Button_Icon[5].first = 0;
			m_Button_Icon[5].second = IC_MedicMana;
		}

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::Factory()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (CKeyMgr::Get_Instance()->GetKeyState('V'))
			m_Button_Icon[0].first = 1;
		else
			m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Vulture;

		if (static_cast<CFactory*>(m_pUintlist->front())->GetIsAddOn())
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('T'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
		}
		else
			m_Button_Icon[1].first = 4;
		m_Button_Icon[1].second = IC_Tank;

		if (static_cast<CFactory*>(m_pUintlist->front())->GetIsAddOn() &&
			(CGameMgr::Get_Instance()->GetTechCount(TECH_Armory) > 0))
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('G'))
				m_Button_Icon[2].first = 1;
			else
				m_Button_Icon[2].first = 0;
		}
		else
			m_Button_Icon[2].first = 4;
		m_Button_Icon[2].second = IC_Goliath;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		if (CKeyMgr::Get_Instance()->GetKeyState('C'))
			m_Button_Icon[6].first = 1;
		else
			m_Button_Icon[6].first = 0;
		m_Button_Icon[6].second = IC_FactoryAddon;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::Starport()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (CKeyMgr::Get_Instance()->GetKeyState('W'))
			m_Button_Icon[0].first = 1;
		else
			m_Button_Icon[0].first = 0;
		m_Button_Icon[0].second = IC_Wirse;

		m_Button_Icon[1].first = 4;
		m_Button_Icon[1].second = IC_Dropship;

		if (CKeyMgr::Get_Instance()->GetKeyState('V'))
			m_Button_Icon[2].first = 1;
		else
			m_Button_Icon[2].first = 4;
		m_Button_Icon[2].second = IC_Vassle;

		m_Button_Icon[3].first = 4;
		m_Button_Icon[3].second = IC_BattleCulsor;

		m_Button_Icon[4].first = 4;
		m_Button_Icon[4].second = IC_Vakily;

		m_Button_Icon[5].first = 0;
		m_Button_Icon[5].second = IC_Return;

		if (CKeyMgr::Get_Instance()->GetKeyState('C'))
			m_Button_Icon[6].first = 1;
		else
			m_Button_Icon[6].first = 0;
		m_Button_Icon[6].second = IC_StarportAddon;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::Armory()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (ChekList_OBJ(OT_Mecha_AtkUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Mecha_AtkUp))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('W'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Mecha_AtkUp;
		}

		if (ChekList_OBJ(OT_Sky_AtkUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Sky_AtkUp))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('S'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_Sky_AtkUp;
		}


		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		if (ChekList_OBJ(OT_Mecha_DefUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Mecha_DefUp))
		{
			m_Button_Icon[3].first = 99;
			m_Button_Icon[3].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('P'))
				m_Button_Icon[3].first = 1;
			else
				m_Button_Icon[3].first = 0;
			m_Button_Icon[3].second = IC_Mecha_DefUp;
		}

		if (ChekList_OBJ(OT_Sky_DefUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Sky_DefUp))
		{
			m_Button_Icon[4].first = 99;
			m_Button_Icon[4].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('H'))
				m_Button_Icon[4].first = 1;
			else
				m_Button_Icon[4].first = 0;
			m_Button_Icon[4].second = IC_Sky_DefUp;
		}

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::Siencefacilly()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (ChekList_OBJ(OT_Vessle_Emp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Emp))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('E'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Vasse_Emp;
		}

		if (ChekList_OBJ(OT_Vessle_Irradiate) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Irradiate))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('I'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_Vasse_Eraady;
		}

		if (ChekList_OBJ(OT_Vessle_Mana) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vessle_Mana))
		{
			m_Button_Icon[2].first = 99;
			m_Button_Icon[2].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('T'))
				m_Button_Icon[2].first = 1;
			else
				m_Button_Icon[2].first = 0;
			m_Button_Icon[2].second = IC_Vasse_Mana;
		}


		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		if (CKeyMgr::Get_Instance()->GetKeyState('C'))
			m_Button_Icon[6].first = 1;
		else
			m_Button_Icon[6].first = 0;
		m_Button_Icon[6].second = IC_CoOvp_Addon;

		m_Button_Icon[7].first = 0;
		m_Button_Icon[7].second = IC_BattleAddon;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::FactoryAddon()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{

		if (ChekList_OBJ(OT_Vulture_Speed) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vulture_Speed))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('I'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Vulture_SpeedUp;
		}
		
		if (ChekList_OBJ(OT_Vulture_mine) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Vulture_mine))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('M'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_Vulture_MineUp;
		}
		
		if (ChekList_OBJ(OT_Tank_SiegeMod) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Tank_SiegeMod))
		{
			m_Button_Icon[2].first = 99;
			m_Button_Icon[2].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('S'))
				m_Button_Icon[2].first = 1;
			else
				m_Button_Icon[2].first = 0;
			m_Button_Icon[2].second = IC_Tank_SiegeUp;

		}

		if (ChekList_OBJ(OT_Gholiat_SightUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Gholiat_SightUp))
		{
			m_Button_Icon[3].first = 99;
			m_Button_Icon[3].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('C'))
				m_Button_Icon[3].first = 1;
			else
				m_Button_Icon[3].first = 0;
			m_Button_Icon[3].second = IC_Goliath_SightUp;
		}
		
		

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::StarportAddon()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (ChekList_OBJ(OT_Wirse_Cloak) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Wirse_Cloak))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('C'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Wirse_Cloak;
		}
		
		if (ChekList_OBJ(OT_Wires_Mana) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Wires_Mana))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('A'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_Wirse_Mana;

		}
		
		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99; 

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99; 

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::CovertOps()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (ChekList_OBJ(OT_Ghost_LockDown) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Ghost_LockDown))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('L'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Ghost_LockDownUp;
		}
		

		if (ChekList_OBJ(OT_Ghost_Cloak) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Ghost_Cloak))
		{
			m_Button_Icon[1].first = 99;
			m_Button_Icon[1].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('C'))
				m_Button_Icon[1].first = 1;
			else
				m_Button_Icon[1].first = 0;
			m_Button_Icon[1].second = IC_Ghost_CloakUp;
		}

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		if (ChekList_OBJ(OT_Ghost_SightUp) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Ghost_SightUp))
		{
			m_Button_Icon[3].first = 99;
			m_Button_Icon[3].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('O'))
				m_Button_Icon[3].first = 1;
			else
				m_Button_Icon[3].first = 0;
			m_Button_Icon[3].second = IC_Ghost_SightUp;
		}

		if (ChekList_OBJ(OT_Ghost_Mana) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Ghost_Mana))
		{
			m_Button_Icon[4].first = 99;
			m_Button_Icon[4].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('M'))
				m_Button_Icon[4].first = 1;
			else
				m_Button_Icon[4].first = 0;
			m_Button_Icon[4].second = IC_Ghost_Mana;
		}

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

void CIconUI::cmdNuke()
{
	if (m_buildState == BS_TEMP || m_buildState == BS_MAKE || m_buildState == BS_LINK)
	{
		m_Button_Icon[0].first = 99;
		m_Button_Icon[0].second = 99;

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 0;
		m_Button_Icon[8].second = IC_Cancel;
	}
	else
	{
		if (ChekList_OBJ(OT_Cmd_Nuke) ||
			CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Cmd_Nuke))
		{
			m_Button_Icon[0].first = 99;
			m_Button_Icon[0].second = 99;
		}
		else
		{
			if (CKeyMgr::Get_Instance()->GetKeyState('N'))
				m_Button_Icon[0].first = 1;
			else
				m_Button_Icon[0].first = 0;
			m_Button_Icon[0].second = IC_Cmd_NukeUp;
		}
		

		m_Button_Icon[1].first = 99;
		m_Button_Icon[1].second = 99;

		m_Button_Icon[2].first = 99;
		m_Button_Icon[2].second = 99;

		m_Button_Icon[3].first = 99;
		m_Button_Icon[3].second = 99;

		m_Button_Icon[4].first = 99;
		m_Button_Icon[4].second = 99;

		m_Button_Icon[5].first = 99;
		m_Button_Icon[5].second = 99;

		m_Button_Icon[6].first = 99;
		m_Button_Icon[6].second = 99;

		m_Button_Icon[7].first = 99;
		m_Button_Icon[7].second = 99;

		m_Button_Icon[8].first = 99;
		m_Button_Icon[8].second = 99;
	}
}

bool CIconUI::ChekList_OBJ(OBJ_TYPE _id)
{

	for (auto ID : *m_listSpawn)
	{
		if (ID == _id)
			return true;
	}
	return false;
}
