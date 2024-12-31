#include "pch.h"
#include "CMapMgr.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CObj.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CResource.h"
#include "CResourceMgr.h"

CMapMgr* CMapMgr::m_pInstance = nullptr;

void CMapMgr::Initialize_Map()
{
    CTileMgr::Get_Instance()->Load_Tile(); // 로드 타일
    CResourceMgr::Get_Instance()->Load_Resorce(); //로드 가스 & 미네랄 

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

    vector<CResource*> vecResouce = *(CResourceMgr::Get_Instance()->Get_Vec());

    for (auto& Resource : vecResouce)
    {
        Resource->Update_Rect();

        Pos pos = { (int)(Resource->GetRect().top) / 32,(int)(Resource->GetRect().left) / 32 };

        if (Resource->GetOption() < 2)
        {
           
            for (int i = 0; i < 64 / 32; i++)
            {
                for (int j = 0; j < 64 / 32; j++)
                {
                    m_Map[pos.y + i][pos.x + j] = 2;
                }
            }
        }
        else
        {
            for (int i = 0; i < 64 / 32; i++)
            {
                for (int j = 0; j < 128 / 32; j++)
                {
                    m_Map[pos.y + i][pos.x + j] = 3;
                }
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
