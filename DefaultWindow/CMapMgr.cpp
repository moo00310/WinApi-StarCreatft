#include "pch.h"
#include "CMapMgr.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CObj.h"


CMapMgr* CMapMgr::m_pInstance = nullptr;

CMapMgr::CMapMgr() :m_dwTime(0)
{
}

CMapMgr::~CMapMgr()
{
}

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
                    map[i][j] = pTile->Get_Option(); 
                 }

                cout<< endl;
                index++;
            }
        }
    }
}
