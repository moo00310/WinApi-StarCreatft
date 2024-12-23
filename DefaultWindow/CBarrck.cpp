#include "pch.h"
#include "CBarrck.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"

CBarrck::CBarrck()
{
}

CBarrck::~CBarrck()
{
    Release();
}

void CBarrck::Initialize()
{
    // 맵의 주소를 받아옴
    m_Map = CMapMgr::Get_Instance()->GetMap();
    m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/Barracks.bmp", L"Barrck");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Build/BuildTemplate.bmp", L"BuildTemplate");

    m_tInfo.fCX = 192.f;
    m_tInfo.fCY = 160.f;

    m_pImgKey = L"BuildTemplate";
    m_iTemplateSize = 2;
    m_bTemplate = true;
    m_eCurState_Build = BS_TEMP;
    m_eObjID = OT_Barrck;
    m_tStat = { 1000.f, 1.f, 0, 1, 0, 0.f, 80 , DF_LAGE, AT_END };
    m_eRender = RENDER_GAMEOBJECT;
     
    //m_iMyBuildTIme = get<3>(ObjCost.at(OT_Barrck));
    m_iMyBuildTIme = 80;

    __super::Update_Rect();
    Block_Map();
}

int CBarrck::Update()
{
    if (m_bDead || m_tStat.m_iHp <= 0)
    {
        // 터지는이펙트 & 사운드

        UnBlock_Map(); // 바닥 이동 불가 해제
        return OBJ_DEAD;
    }

    KeyInput();
    Spawn_Uint_CoolDown();     // 쿨타임 적용해서 유닛생성

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBarrck::Late_Update()
{
    Change_Motion();

    if (m_bTemplate) return;
    __super::Move_Frame();
}

void CBarrck::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
 

    if (m_bTemplate)
     {
        int CX = (int)m_tInfo.fX - (int)(BuildTemplate_Size.x * 0.5f);
        int CY = (int)m_tInfo.fY - (int)(BuildTemplate_Size.y * 0.5f);

       GdiTransparentBlt(hDC,
           CX + iScrollX,	
           CY + iScrollY,
           (int)BuildTemplate_Size.x,			
           (int)BuildTemplate_Size.y,
           hMemDC,						
           (int)BuildTemplate_Size.x * m_iTemplateSize, 
           (int)BuildTemplate_Size.y * m_tFrame.iCurCount,
           (int)BuildTemplate_Size.x,									
           (int)BuildTemplate_Size.y,
           RGB(0, 255, 0));		
     }
     else
     {
       GdiTransparentBlt(hDC,			// 복사 받을 DC
           m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
           m_tRect.top + iScrollY,
           (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
           (int)m_tInfo.fCY,
           hMemDC,						// 복사할 이미지 DC	
           0,           // 비트맵 출력 시작 좌표(Left, top)
           (int)m_tInfo.fCY * m_tFrame.iCurCount,
           (int)m_tInfo.fCX,		// 복사할 이미지의 가로, 세로
           (int)m_tInfo.fCY,
           RGB(0, 255, 0));		// 제거할 색상
    }


}

void CBarrck::Release()
{
  
}

void CBarrck::Change_Motion()
{
    if (m_eCurState_Build == BS_TEMP)
    {
        m_pImgKey = L"BuildTemplate";

        if (m_iBuildCount < (m_iMyBuildTIme / 5) * 1)
            m_tFrame.iCurCount = 0;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 2)
            m_tFrame.iCurCount = 1;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 3)
            m_tFrame.iCurCount = 2;
        else if (m_iBuildCount < (m_iMyBuildTIme / 5) * 4)
        {
            m_bTemplate = false;
            m_eCurState_Build = BS_MAKE;
        }
            
        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp/ m_iMyBuildTIme);
    }

  
    if (m_eCurState_Build == BS_MAKE)
    {
        m_pImgKey = L"Barrck";
        m_tFrame.iFrameStart = 0;
        m_tFrame.iCurCount = 0;
        m_tFrame.iFrameEnd = 0;
        if (m_iMyBuildTIme < m_iBuildCount)
            m_eCurState_Build = BS_IDLE;

        m_iBuildCount++;
        Add_Stat_hp(m_tStat.m_iMaxHp / m_iMyBuildTIme);
    }
    
   if (m_ePreState_Bulid != m_eCurState_Build)
    {
       switch (m_eCurState_Build)
       {
       case BS_IDLE:
           m_tFrame.iFrameStart = 1;
           m_tFrame.iFrameEnd = 1;
           m_tFrame.iCurCount = 1;
           m_tFrame.dwSpeed = 200;
           m_tFrame.dwTime = GetTickCount64();
           break;
       case BS_RUN:
           m_tFrame.iFrameStart = 2;
           m_tFrame.iFrameEnd = 3;
           m_tFrame.iCurCount = 2;
           m_tFrame.dwSpeed = 200;
           m_tFrame.dwTime = GetTickCount64();
           break;
       case BS_END:
           break;
       default:
           break;
       }

       m_ePreState_Bulid = m_eCurState_Build;
    }
}

void CBarrck::Block_Map()
{

    Pos pos = { (m_tRect.top ) / 32,(m_tRect.left ) / 32 };

    for (int i = 0; i < m_tInfo.fCY / 32; i++)
    {
        for (int j = 0; j < m_tInfo.fCX / 32; j++)
        {
            Pos temp = {i,j};
            CMapMgr::Get_Instance()->SetTileType(pos + temp, 2);
        }
    }
}

void CBarrck::UnBlock_Map()
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

void CBarrck::KeyInput()
{
    if (!m_bSelect) return;

    if (m_eCurState_Build == BS_MAKE ||
        m_eCurState_Build == BS_TEMP) return;

    // 마린 생산
    if (CKeyMgr::Get_Instance()->Key_Down('A'))
    {
        if (m_listSpawn.size() < 5)
            m_listSpawn.push_back(OT_Marine);
    }

    // 메딕 생산
    if (CKeyMgr::Get_Instance()->Key_Down('E'))
    {
        //m_queSpawn.push(OT_Medic);
    }

    // 고스트 생산
    if (CKeyMgr::Get_Instance()->Key_Down('G'))
    {
        //m_queSpawn.push(OT_Ghost);
    }

}
