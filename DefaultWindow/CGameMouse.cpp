#include "pch.h"
#include "CGameMouse.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CMarine.h"
#include "CCollisionMgr.h"
#include "CMapMgr.h"
#include "CScv.h"
#include "CGameMgr.h"
#include "CSoundMgr.h"


/*---------------
    GameMouse
--------------------*/

CGameMouse::CGameMouse() : m_eCurState(MS_IDLE), m_ePreState(MS_IDLE), m_indexY(0), m_UnitList(nullptr),
m_Select_UnitList(nullptr), isDrag(false), m_BuildList(nullptr), isBuildMod(false), m_eBuildType(OT_END),
m_pImgKey_build(nullptr), m_iBuild_Index(0), m_UnitList_E(nullptr), m_BuildList_E(nullptr)
{
    ZeroMemory(&ptMouse, sizeof(POINT));
    ZeroMemory(&m_DragStart, sizeof(POINT));
    ZeroMemory(&m_DragEnd, sizeof(POINT));
    ZeroMemory(&m_tColor, sizeof(COLOR));
}

CGameMouse::~CGameMouse()
{
    Release();
}

void CGameMouse::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Mouse/Cursor.bmp", L"Cursor");

    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    m_pImgKey = L"Cursor";

    m_UnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
    m_BuildList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD);

    m_UnitList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
    m_BuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

    m_Select_UnitList = CObjMgr::Get_Instance()->Get_Select_List();
}

int CGameMouse::Update()
{
    m_tInfo.fX = (float)ptMouse.x;
    m_tInfo.fY = (float)ptMouse.y;

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    ScrollMove(ptMouse);
    SetScroll(); 
    DrawBulid();

    MouseInput(ptMouse);

    ColObject();
    Change_Cursor();

    // 마우스 잠굼
    //LockMouse();
   
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CGameMouse::Late_Update()
{
    ShowCursor(FALSE);
    CMouse::Move_Frame();
}

void CGameMouse::Render(HDC hDC)
{
    if (isBuildMod)
    {
        HDC		hBuildDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey_build);
        GdiTransparentBlt(hDC,		
            (int)(m_tRect.left/32.f) * 32,	
            (int)(m_tRect.top/32.f) * 32,
            (int)m_tInfo.fCX,			
            (int)m_tInfo.fCY,
            hBuildDC,
            0,
            (int)m_tInfo.fCY * m_iBuild_Index,
            (int)m_tInfo.fCX,					
            (int)m_tInfo.fCY,
            RGB(m_tColor.R, m_tColor.G, m_tColor.B));

    }
    else
    {
        m_tInfo.fCX = 50.f;
        m_tInfo.fCY = 50.f;

        // 일반 모드
        HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

        GdiTransparentBlt(hDC,			// 복사 받을 DC
            m_tRect.left,	// 복사 받을 위치 좌표 X, Y	
            m_tRect.top,
            (int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
            (int)m_tInfo.fCY,
            hMemDC,						// 복사할 이미지 DC	
            (int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
            (int)m_tInfo.fCY * m_indexY,
            (int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));

        /*---------------
            드래그
        -------------------*/
        if (!isDrag) return;
        HPEN newPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

        HPEN oldPen = (HPEN)SelectObject(hDC, newPen);
        HPEN oldBrush = (HPEN)SelectObject(hDC, newBrush);

        Rectangle(hDC, (int)m_DragStart.x, (int)m_DragStart.y, (int)m_DragEnd.x, (int)m_DragEnd.y);


        SelectObject(hDC, oldPen);
        SelectObject(hDC, oldBrush);

        DeleteObject(newBrush);
        DeleteObject(newPen);
    }
}

void CGameMouse::Release()
{
}

void CGameMouse::ClearList()
{
    // 유닛 선택 초기화
    for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
        {
            if (unit != nullptr)
                unit->Set_Select(false);
        });
    m_Select_UnitList->clear();
}

void CGameMouse::ClearDrag()
{
    ZeroMemory(&m_DragStart, sizeof(POINT));
    ZeroMemory(&m_DragEnd, sizeof(POINT));
}

void CGameMouse::MouseInput(POINT ptMouse)
{
    ///////////////////////////////////
    Pos temp = { (int)(ptMouse.y - CScrollMgr::Get_Instance()->Get_ScrollY()) / TILECY , int(ptMouse.x - CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECY };

    if (isBuildMod)
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            if (AbleBuild())  // 건물을 지울 수 있는 곳인지 아닌지 체크
            {
                if (CGameMgr::Get_Instance()->isBuying(m_eBuildType))
                {
                    isBuildMod = false;
                    if (auto* pUnit = dynamic_cast<CScv*>(m_Select_UnitList->front()))
                    {
                        pUnit->Astar(temp);
                        pUnit->SetInput(IP_BUILD);
                        pUnit->SetPos(temp);
                    }
                }
            }
            else
            {
                CSoundMgr::Get_Instance()->PlaySFX(L"tscerr01.wav", 0.8f);
            } 
        }

        // 우클릭으로 취소
        if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
        {
            isBuildMod = false;
            ClearList();
        }
    }
    else
    {
        ///// 우클릭 : MOVE 
        if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
        {
            m_eCurState = MS_MOVE;
            if (m_Select_UnitList->size() == 1)
            {
                if (m_Select_UnitList->front() != nullptr && !m_Select_UnitList->front()->GetIsEnemy())
                {
                    if (auto* pUnit = dynamic_cast<CUnit*>(m_Select_UnitList->front()))
                    {
                        if (pUnit->GetInput() == IP_BUILD) return;
                        pUnit->Astar(temp);
                        pUnit->SetInput(IP_MOVE);
                    }
                }
            }
            else if (m_Select_UnitList->size() > 1)
            {
                Pos IndexArraay[12] = {};
                int array(0);
//#pragma region 가장 가까운 놈 기준 부대이동
//                // 마우스랑 가장 가까운 유닛 찾기
//                Pos BestIndex = CCollisionMgr::Collision_Neares_Unit_pos(temp, *m_Select_UnitList);
//
//                // 가장 베스트 인덱스에서 빼기
//                for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
//                    {
//                        Pos pos = { (int)unit->Get_Scroll_Info().fY / 32, (int)unit->Get_Scroll_Info().fX / 32 };
//                        IndexArraay[array] = BestIndex - pos;
//                        array++;
//                    });
//
//
//                // 마우스 포인트 위치에서 각각 정해진 위치로 이동
//                array = 0;
//                for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
//                    {
//
//                        if (auto* pUnit = dynamic_cast<CUnit*>(unit))
//                        {
//                            pUnit->Astar(temp - IndexArraay[array]);
//                            pUnit->SetInput(IP_MOVE);
//                            array++;
//                        }
//
//                    });
//#pragma endregion
// 
                // 마우스 포인트 위치에서 각각 정해진 위치로 이동
                
                for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
                    {
                        Pos nextPos = temp + MoveFront[array];

                        if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                        {
                            if (pUnit->GetInput() == IP_BUILD) return;
                            pUnit->Astar(nextPos);
                            pUnit->SetInput(IP_MOVE);
                            array++;
                        }

                    });
               
            }
        }
        if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
        {
            m_eCurState = MS_IDLE;
        }

        //// A - 좌클릭 : 어택 땅
        if (m_eCurState == MS_ATTACK && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            m_eCurState = MS_IDLE;

            for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
                {
                    if (unit != nullptr && !m_Select_UnitList->front()->GetIsEnemy())
                    {
                        if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                        {
                            if (pUnit->GetInput() == IP_BUILD) return;
                            pUnit->Astar(temp);
                            pUnit->SetAGroundPos(temp);
                            pUnit->SetInput(IP_ATTACK);
                        }
                    }
                });
        }

        // 땅 좌클릭
        if (m_eCurState == MS_IDLE && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            ClearList();
            m_DragStart.x = ptMouse.x;
            m_DragStart.y = ptMouse.y;
            isDrag = true;
        }
        if (isDrag == true && CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            m_eCurState = MS_DRAG;
            m_DragEnd.x = ptMouse.x;
            m_DragEnd.y = ptMouse.y;
        }

        if (isDrag == true && CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            isDrag = false;
            ColDrag();
            ClearDrag();
            m_eCurState = MS_IDLE;
        }

        if (CKeyMgr::Get_Instance()->Key_Down('A'))
        {
            m_eCurState = MS_ATTACK;
        }

        if (CKeyMgr::Get_Instance()->Key_Down('S'))
        {
            for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
                {
                    if (unit != nullptr && !m_Select_UnitList->front()->GetIsEnemy())
                    {
                        if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                        {
                            pUnit->SetInput(IP_STOP);
                        }
                    }
                });
        }

        if (CKeyMgr::Get_Instance()->Key_Down('H'))
        {
            for_each(m_Select_UnitList->begin(), m_Select_UnitList->end(), [&](CObj* unit)
                {
                    if (unit != nullptr && !m_Select_UnitList->front()->GetIsEnemy())
                    {
                        if (auto* pUnit = dynamic_cast<CUnit*>(unit))
                        {
                            pUnit->SetInput(IP_HOLD);
                        }
                    }
                });
        }
    }
}

void CGameMouse::ScrollMove(POINT mouse)
{
    if (mouse.x >=  WINCX - 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
        m_eCurState = MS_SCROLL_R;
    }
    else if (m_eCurState == MS_SCROLL_R)
    {
        m_eCurState = MS_IDLE;
    }

    if (mouse.x <= 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
        m_eCurState = MS_SCROLL_L;
    }
    else if (m_eCurState == MS_SCROLL_L)
    {
        m_eCurState = MS_IDLE;
    }

    if (mouse.y >= WINCY -20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);
       m_eCurState = MS_SCROLL_D;
    }
    else if (m_eCurState == MS_SCROLL_D)
    {
        m_eCurState = MS_IDLE;
    }
    if (mouse.y <= 20)
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
        m_eCurState = MS_SCROLL_U;
    }
    else if (m_eCurState == MS_SCROLL_U)
    {
        m_eCurState = MS_IDLE;
    }
}

void CGameMouse::ColObject()
{
    if (isDrag) return;
    if (isBuildMod) return;

    CObj* Obj(nullptr);
    // 마우스랑 충돌했는지 검사
    if ((Obj = CCollisionMgr::Collision_Rect_Mouse(m_tRect, *m_UnitList, *m_BuildList, * m_UnitList_E, *m_BuildList_E)) != nullptr)
    {
        if (Obj->GetIsEnemy())
            m_eCurState = MS_OBJ_E;
        else
            m_eCurState = MS_OBJ;

        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            ClearList();
            Obj->Set_Select(true);
            CObjMgr::Get_Instance()->Add_SelectList(Obj);
        }
           
    }
    else if (MS_OBJ == m_eCurState)
    {
        m_eCurState = MS_IDLE;
    }
}

void CGameMouse::Change_Cursor()
{
    if (isBuildMod) return;
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case MS_IDLE:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.iCurCount = 0;
            m_indexY = 0;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_OBJ:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.iCurCount = 0;
            m_indexY = 2;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_OBJ_E:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.iCurCount = 0;
            m_indexY = 4;
            m_tFrame.dwSpeed = 200;
            break;

        case MS_ATTACK:
            m_tFrame.iFrameStart = 10;
            m_tFrame.iFrameEnd = 10;
            m_tFrame.iCurCount = 10;
            m_indexY = 5;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_MOVE:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.iCurCount = 0;
            m_indexY = 5;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_DRAG:
            m_tFrame.iFrameStart = 5;
            m_tFrame.iFrameEnd = 5;
            m_tFrame.iCurCount = 5;
            m_indexY = 0;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_R:
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 1;
            m_tFrame.iCurCount = 0;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_U:
            m_tFrame.iFrameStart = 4;
            m_tFrame.iFrameEnd = 5;
            m_tFrame.iCurCount = 4;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_L:
            m_tFrame.iFrameStart = 8;
            m_tFrame.iFrameEnd = 9;
            m_tFrame.iCurCount = 8;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case MS_SCROLL_D:
            m_tFrame.iFrameStart = 12;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.iCurCount = 12;
            m_indexY = 1;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;

        }

        m_ePreState = m_eCurState;
    }

}

void CGameMouse::ColDrag()
{
    float left = min((float)m_DragStart.x, (float)m_DragEnd.x);
    float right = max((float)m_DragStart.x, (float)m_DragEnd.x);
    float top = min((float)m_DragStart.y, (float)m_DragEnd.y);
    float bottom = max((float)m_DragStart.y, (float)m_DragEnd.y);

    RECT rc = { (LONG)left, (LONG)top, (LONG)right, (LONG)bottom };
    CCollisionMgr::Collision_Rect_Mouse_RECT(rc,*m_UnitList, m_Select_UnitList);
}


void CGameMouse::DrawBulid()
{
    if (!isBuildMod) return;

    switch (m_eBuildType)
    {
    case OT_Commend:
        m_pImgKey_build = L"CommandCenter";
        m_tInfo.fCX = 128.f;
        m_tInfo.fCY = 160.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Suffly:
        m_pImgKey_build = L"SupplyDepot";
        m_tInfo.fCX = 96.f;
        m_tInfo.fCY = 128.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Refinery:
        m_pImgKey_build = L"Refinery";
        m_tInfo.fCX = 192.f;
        m_tInfo.fCY = 192.f;
        m_iBuild_Index = 4;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Barrck:
        m_pImgKey_build = L"Barrck";
        m_tInfo.fCX = 192.f;
        m_tInfo.fCY = 160.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Academy:
        m_pImgKey_build = L"Academy";
        m_tInfo.fCX = 96.f;
        m_tInfo.fCY = 128.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Factory:
        m_pImgKey_build = L"Factory";
        m_tInfo.fCX = 128.f;
        m_tInfo.fCY = 160.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_Addon:
        break;
    case OT_Armory:
        m_pImgKey_build = L"Armory";
        m_tInfo.fCX = 160.f;
        m_tInfo.fCY = 128.f;
        m_iBuild_Index = 1;
        m_tColor.R = 255;
        m_tColor.G = 0;
        m_tColor.B = 255;
        break;
    case OT_Starport:
        m_pImgKey_build = L"Starport";
        m_tInfo.fCX = 128.f;
        m_tInfo.fCY = 160.f;
        m_iBuild_Index = 1;
        m_tColor.R = 0;
        m_tColor.G = 255;
        m_tColor.B = 0;
        break;
    case OT_StarportAddOn:
        break;
    case OT_ScienceFacility:
        m_pImgKey_build = L"ScienceFacility";
        m_tInfo.fCX = 128.f;
        m_tInfo.fCY = 100.f;
        m_iBuild_Index = 1;
        m_tColor.R = 255;
        m_tColor.G = 0;
        m_tColor.B = 255;
        break;
    case OT_ScienceSecret:
        break;
    case OT_CmdNuke:
        break;
    case OT_Build_End:
        break;
    case OT_END:
        break;
    default:
        break;
    }

}

bool CGameMouse::AbleBuild()
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    Pos pos = {  (m_tRect.top  - iScrollY) / 32,(m_tRect.left - iScrollX) / 32 };

    if (m_eBuildType == OT_Refinery)
    {
        for (int i = 1; i < 3; i++)
        {
            for (int j = 1; j < 5; j++)
            {
                Pos temp = { i,j };

                if (CMapMgr::Get_Instance()->GetTileType(pos + temp) != 3)
                {
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        for (int i = 0; i < m_tInfo.fCY / 32; i++)
        {
            for (int j = 0; j < m_tInfo.fCX / 32; j++)
            {
                Pos temp = { i,j };

                if (CMapMgr::Get_Instance()->GetTileType(pos + temp) > 1)
                {
                    return false;
                }

            }
        }

    }

    
    return true;
}


