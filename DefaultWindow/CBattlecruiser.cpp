#include "pch.h"
#include "CBattlecruiser.h"
#include "CObjMgr.h"
#include "CMapMgr.h"
#include "CSoundMgr.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CBulletEffect.h"
#include "CAbstractFactory.h"

void CBattlecruiser::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

	m_pImgKey = L"Battlecruiser";
	m_eObjID = OT_Battlecruiser;
	m_tStat = { 500.f, 500.f, 25, 3, 256, 1.1f, 1300 , DF_LAGE, AT_NORMAL };

	m_iAttackFrame = 0;

	m_eRender = RENDER_SKYOBJ;
	m_tInfo.fCX = 120.f;
	m_tInfo.fCY = 120.f;

	A_GroundPos.x = (int)m_tInfo.fX / 32;
	A_GroundPos.y = (int)m_tInfo.fY / 32;
}

int CBattlecruiser::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"BatteleDeath.mp3", 0.3f);
		//CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}

	KeyInput();
	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBattlecruiser::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();

	if (AttackCoolTime + 1700 < GetTickCount64())
	{
		m_isAttack = false;
		AttackCoolTime = GetTickCount64();
	}
}

void CBattlecruiser::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_5");
	HDC		hShadeDC = CBmpMgr::Get_Instance()->Find_Image(L"BattleShade");

	//그림자
	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY + 30,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hShadeDC,						// 복사할 이미지 DC	
		0, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX +10 ,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 30,
			96,			// 복사 받을 이미지의 가로, 세로
			96,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			96,										// 복사할 이미지의 가로, 세로
			96,
			RGB(255, 0, 255));		// 제거할 색상
	}


	// 몸체
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
		RGB(0, 0, 0));		// 제거할 색상
}

void CBattlecruiser::Release()
{
}

void CBattlecruiser::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBattlecruiser::KeyInput()
{
	// 야마토
	/*if (CKeyMgr::Get_Instance()->Key_Down('N'))
	{

	}*/
}

void CBattlecruiser::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT)
		m_eCurState = STATE_SHOOT;
	else
		m_eCurState = STATE_ATTACK;

	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() && !m_isAttack)
	{
		CSoundMgr::Get_Instance()->PlaySFX(L"BattleAttack.mp3", 0.8f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBattleAttack>::CreateBattleAtk(this, _Enemey));
		m_isAttack = true;
		m_AttackTime = GetTickCount64();
	}
}

bool CBattlecruiser::CanGo(Pos pos)
{
	if (pos.y >= 128 || pos.x >= 128) return false;
	if (CMapMgr::Get_Instance()->GetTileType(pos) >= 0)
		return true;
	else
		return false;
}
