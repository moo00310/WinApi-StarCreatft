#include "pch.h"
#include "CSpawnUI.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CBuild.h"

CSpawnUI::CSpawnUI(): IndexX(0), IndexY(0), m_SpawnList(nullptr), m_eID(OT_END)
{
}

CSpawnUI::~CSpawnUI()
{
}

void CSpawnUI::Initialize()
{
   m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
    
	m_tInfo = { 400.f, 540.f ,194.f, 95.f };
}

int CSpawnUI::Update()
{
	if (m_pUintlist->size() != 1)
	{
		m_bRender = false;
		return 0;
	}

	m_eID = m_pUintlist->front()->Get_ObjID();

	if (m_eID == OT_Suffly || m_eID == OT_Refinery)
	{
		m_bRender = false;
		return 0;
	}
		

	if (m_eID > OT_Unit_End)
	{
		CBuild* bulid = static_cast<CBuild*>(m_pUintlist->front());
		if (bulid->Get_State() == BS_IDLE || bulid->Get_State() == BS_RUN)
		{
			m_SpawnList = bulid->Get_SpawnList();
			m_bRender = true;
		}
		else
		{
			m_bRender = false;
		}
	}
	else
	{
		m_bRender = false;
	}
	
	__super::Update_Rect();
    return 0;
}

void CSpawnUI::Late_Update()
{
}

void CSpawnUI::Render(HDC hdc)
{
	if (m_pUintlist->empty()) return;
    if (m_bRender && !m_pUintlist->front()->GetIsEnemy())
    {
		/*--------------
		  1-2-3-4-5 템플릿 출력
		-----------------*/
		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ETC");

		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top,
			194,			// 복사 받을 이미지의 가로, 세로
			95,
			hMemDC,						// 복사할 이미지 DC	
			0,
			0,
			194,
			95,
			RGB(0, 255, 255));

		if (m_SpawnList->empty()) return;

		/*-----------------
		 리스트 진행바 출력
		-----------------*/
		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left + 50,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + 30,
			134,			// 복사 받을 이미지의 가로, 세로
			12,
			hMemDC,						// 복사할 이미지 DC	
			0,
			144,
			134,
			12,
			RGB(0, 255, 255));

		if (m_pUintlist->empty()) return;

		float m_fProgress = static_cast<CBuild*>(m_pUintlist->front())->GetProgress();

		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left + 50,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + 30,
			(int)m_fProgress,			// 복사 받을 이미지의 가로, 세로
			12,
			hMemDC,						// 복사할 이미지 DC	
			0,
			158,
			(int)m_fProgress,
			12,
			RGB(0, 255, 255));


		/*--------------
		 리스트 정보 출력
		-----------------*/

		HDC		hQueDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonIcon");

		int count(0);
		for_each(m_SpawnList->begin(), m_SpawnList->end(), [&](OBJ_TYPE _temp)
			{
				Change_SpawnIcon(_temp);

				GdiTransparentBlt(hdc,
					m_tRect.left + 0 + (49 * Spawn_index[count].x),
					m_tRect.top  + 5 + (50 * Spawn_index[count].y),
					44,
					44,
					hQueDC,
					44 * IndexX,
					44 * IndexY,
					44,
					44,
					RGB(0, 255, 0));

				count++;
		});

    }
}

void CSpawnUI::Release()
{
}

void CSpawnUI::Change_SpawnIcon(OBJ_TYPE _type)
{
	switch (_type)
	{
	case OT_Scv:
		IndexX = 2;
		IndexY = 11;
		break;
	case OT_Marine:
		IndexX = 2;
		IndexY = 12;
		break;
	case OT_Medic:
		IndexX = 2;
		IndexY = 42;
		break;
	case OT_Ghost:
		IndexX = 2;
		IndexY = 14;
		break;
	case OT_Tank:
		IndexX = 2;
		IndexY = 16;
		break;
	case OT_Battlecruiser:
		IndexX = 2;
		IndexY = 20;
		break;
	case OT_Unit_End:
		break;

	case OT_Addon:
		IndexX = 2;
		IndexY = 39;
		break;
	case OT_StarportAddOn:
		IndexX = 2;
		IndexY = 38;
		break;
	case OT_ScienceSecret:
		IndexX = 2;
		IndexY = 36;
		break;
	case OT_CmdNuke:
		IndexX = 2;
		IndexY = 35;
		break;


	case OT_Marine_SightUp:
		IndexX = 2;
		IndexY = IC_MarineRichUp;
		break;
	case OT_Marine_Streampack:
		IndexX = 2;
		IndexY = IC_MarineSteamPack;
		break;
	case OT_Medic_Magic1:
		IndexX = 2;
		IndexY = IC_MedicMegic_1;
		break;
	case OT_Medic_Magic2:
		IndexX = 2;
		IndexY = IC_MedicMegic_2;
		break;
	case OT_Medic_Mana:
		IndexX = 2;
		IndexY = IC_MedicMana;
		break;
	case OT_Vulture_Speed:
		IndexX = 2;
		IndexY = IC_Vulture_SpeedUp;
		break;
	case OT_Vulture_mine:
		IndexX = 2;
		IndexY = IC_Vulture_MineUp;
		break;
	case OT_Tank_SiegeMod:
		IndexX = 2;
		IndexY = IC_Tank_SiegeUp;
		break;
	case OT_Gholiat_SightUp:
		IndexX = 2;
		IndexY = IC_Goliath_SightUp;
		break;
	case OT_Mecha_AtkUp:
		IndexX = 2;
		IndexY = IC_Mecha_AtkUp;
		break;
	case OT_Sky_AtkUp:
		IndexX = 2;
		IndexY = IC_Sky_AtkUp;
		break;
	case OT_Mecha_DefUp:
		IndexX = 2;
		IndexY = IC_Mecha_DefUp;
		break;
	case OT_Sky_DefUp:
		IndexX = 2;
		IndexY = IC_Sky_DefUp;
		break;
	case OT_Wirse_Cloak:
		IndexX = 2;
		IndexY = IC_Wirse_Cloak;
		break;
	case OT_Wires_Mana:
		IndexX = 2;
		IndexY = IC_Wirse_Mana;
		break;
	case OT_Vessle_Emp:
		IndexX = 2;
		IndexY = IC_Vasse_Emp;
		break;
	case OT_Vessle_Irradiate:
		IndexX = 2;
		IndexY = IC_Vasse_Eraady;
		break;
	case OT_Vessle_Mana:
		IndexX = 2;
		IndexY = IC_Vasse_Mana;
		break;
	case OT_Ghost_LockDown:
		IndexX = 2;
		IndexY = IC_Ghost_LockDownUp;
		break;
	case OT_Ghost_Cloak:
		IndexX = 2;
		IndexY = IC_Ghost_CloakUp;
		break;
	case OT_Ghost_SightUp:
		IndexX = 2;
		IndexY = IC_Ghost_SightUp;
		break;
	case OT_Ghost_Mana:
		IndexX = 2;
		IndexY = IC_Ghost_Mana;
		break;
	case OT_Cmd_Nuke:
		IndexX = 2;
		IndexY = IC_Cmd_NukeUp;
		break;
	case OT_END:
		break;
	default:
		break;
	}
}

