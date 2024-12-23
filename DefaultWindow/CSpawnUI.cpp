#include "pch.h"
#include "CSpawnUI.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CBuild.h"

void CSpawnUI::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ETC.bmp", L"ETC");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/UI/Icon/ButtonIcon.bmp", L"ButtonIcon");
    m_pUintlist = CObjMgr::Get_Instance()->Get_Select_List();
    
	m_tInfo = { 400.f, 540.f ,194.f, 95.f };
}

int CSpawnUI::Update()
{
	if (m_pUintlist->size() == 1)
	{
		m_eID = m_pUintlist->front()->Get_ObjID();
		if (m_eID > OT_Unit_End)
		{
			m_queSpawn = static_cast<CBuild*>(m_pUintlist->front())->Get_Queue();
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
		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ButtonIcon");

		GdiTransparentBlt(hdc,			// 복사 받을 DC
			m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top,
			194,			// 복사 받을 이미지의 가로, 세로
			95,
			hMemDC,						// 복사할 이미지 DC	
			0 ,
			0,
			194,
			95,
			RGB(0, 255, 255));

		/*--------------
		 큐  정보 출력
		-----------------*/

		if (m_queSpawn->empty()) return;

		HDC		hQueDC = CBmpMgr::Get_Instance()->Find_Image(L"ETC");

		m_showQue = *m_queSpawn;

		for (int i = 0; i < m_showQue.size(); i++)
		{
			OBJ_TYPE _temp = m_showQue.front();
			Change_SpawnIcon(_temp);

			GdiTransparentBlt(hdc,			// 복사 받을 DC
				m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
				m_tRect.top,
				194,			// 복사 받을 이미지의 가로, 세로
				95,
				hQueDC,						// 복사할 이미지 DC	
				0,
				0,
				194,
				95,
				RGB(0, 255, 255));
		}
    }
}

void CSpawnUI::Release()
{
}

void CSpawnUI::Change_SpawnIcon(OBJ_TYPE _type)
{


}
