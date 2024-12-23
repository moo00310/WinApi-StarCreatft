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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ETC.bmp", L"ETC");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");
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

	if (m_eID == OT_Suffly || m_eID == OT_StarportAddOn || m_eID == OT_Addon  || m_eID == OT_Refinery)
	{
		m_bRender = true;
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
    if (m_bRender)
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
					m_tRect.left + (49 * Spawn_index[count].x),
					m_tRect.top  + (45 * Spawn_index[count].y),
					44,
					40,
					hQueDC,
					44 * IndexX,
					40 * IndexY,
					44,
					40,
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
		IndexY = 12;
		break;
	case OT_Marine:
		IndexX = 2;
		IndexY = 13;
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
	default:
		break;
	}

}

