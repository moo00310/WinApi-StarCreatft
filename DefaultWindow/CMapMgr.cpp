#include "pch.h"
#include "CMapMgr.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CObj.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CMapMgr* CMapMgr::m_pInstance = nullptr;

void CMapMgr::Initialize_Map()
{

    //// Get_VecTile()에서 벡터 가져오기
     vector<CObj*> vecTile = *(CTileMgr::Get_Instance()->Get_VecTile());
 
    int index = 0; 
    for (int i = 0; i < TILEHIGHT; i++)
    {
        for (int j = 0; j < TILEWIDTH; j++)
        {
            if (index < vecTile.size()) 
            {
                 CTile* pTile = dynamic_cast<CTile*>(vecTile[index]);
                 if (pTile)
                 {
                     // y, x
                     m_Map[i][j] = pTile->Get_Option();
                 }

                cout<< endl;
                index++;
            }
        }
    }
}

void CMapMgr::Render(HDC hdc)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FightSpirit");
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    BitBlt(hdc,
        0, 0, 4096, 4096,
        hMemDC,
        0 - iScrollX,
        0 - iScrollY,
        SRCCOPY);
}
