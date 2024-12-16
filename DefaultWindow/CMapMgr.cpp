#include "pch.h"
#include "CMapMgr.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CObj.h"

CMapMgr* CMapMgr::m_pInstance = nullptr;

CMapMgr::CMapMgr()
{
}

CMapMgr::~CMapMgr()
{
}

void CMapMgr::Initialize()
{
	
	 // Get_VecTile()에서 벡터 가져오기
    vector<CObj*> vecTile = CTileMgr::Get_Instance()->Get_VecTile();
    
    int index = 0; // 1차원 데이터를 2차원 배열에 저장하기 위한 인덱스
    for (int i = 0; i < 75; i++)
    {
        for (int j = 0; j < 75; j++)
        {
            if (index < vecTile.size()) // 벡터 범위 확인
            {
                // dynamic_cast를 사용해 CTile 객체인지 확인
                CTile* pTile = dynamic_cast<CTile*>(vecTile[index]);
                if (pTile)
                {
                    map[i][j] = pTile->Get_Option(); // 배열에 옵션 값 저장
                }
                else
                {
                    // 캐스팅 실패 처리
                    map[i][j] = 0; // 기본값 또는 에러 코드
                }
                index++; // 다음 타일로 이동
            }
            else
            {
                map[i][j] = 0; // 남는 공간에 기본값 할당
            }
        }
    }
}
