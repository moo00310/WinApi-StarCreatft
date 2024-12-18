#include "pch.h"
#include "CMarine.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CMapMgr.h"
#include "CCollisionMgr.h"

CMarine::CMarine() : CUnit(UNIT_MARINE), m_iImgId(0), m_eCurState(STATE_END), m_ePreState(STATE_END)

{
    ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CMarine::~CMarine()
{
	Release();
}

void CMarine::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Marine/Marine.bmp", L"Marine");

    m_pImgKey = L"Marine";
    m_tStat = { 40, 6, 0, 32, 1.8f, 15 , DF_SAMLL, AT_NORMAL };

	m_eCurState = STATE_IDLE;
	m_ePreState = STATE_IDLE;
	m_eDir = DIR_DOWN_RIGHT;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;




	//////////////////////////
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 400.f;



}

int CMarine::Update()
{
	if (m_bDead)
	{
		// 죽음 이펙트
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,CAbstractFactory<CMarineDead>::Create(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}
	
	Update_State();
	MoveTo();
	Attack();
	Test_Key_Input();
	

	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMarine::Late_Update()
{
	Change_Motion();
	__super::Move_Frame();
}

void CMarine::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 255, 0));		// 제거할 색상

}

void CMarine::Release()
{
}

void CMarine::Update_State()
{
}

void CMarine::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_MOVE:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 13;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iCurCount = 11;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CMarine::Test_Key_Input()
{

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		m_eCurState = STATE_ATTACK;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
	{
		if (m_iImgId > 15)
			m_iImgId = 0;
		else
			m_eDir = (DIRECTION)m_iImgId++;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		m_bDead = true;
	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT       ptMouse{};

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		Pos temp = { (ptMouse.y - CScrollMgr::Get_Instance()->Get_ScrollY())/ TILECY , (ptMouse.x - CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECY };
		Astar(temp);
	}
}

void CMarine::MoveTo()
{
	if (m_iPathIndex < _path.size())
	{
		Pos _now = { m_tInfo.fY / TILECY , m_tInfo.fX / TILECY };
		Pos _pos = _path[m_iPathIndex];

		if(_now == _pos)
			++m_iPathIndex;
		else
		{
			// 방향 설정
			Pos dir = (_pos - _now);
			for (int i = 0; i < DIR_END; i++)
			{
				if (dir == MoveFront[i])
				{
					m_eDir = (DIRECTION)i;
					break;
				}
			}
			
			// 단위 벡터로 수정?
			float x(0.f), y(0.f);
			float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
			if (length != 0)
			{
				 x = dir.x / length;
				 y = dir.y / length;
			}

			// 이동
			m_eCurState = STATE_MOVE;
			m_tInfo.fX += m_tStat.m_fSpeed * x;
			m_tInfo.fY += m_tStat.m_fSpeed * y;
		}
	}
	else if(m_iPathIndex == _path.size())
		m_eCurState = STATE_IDLE;
}
