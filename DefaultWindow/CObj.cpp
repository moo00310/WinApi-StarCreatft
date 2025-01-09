#include "pch.h"
#include "CObj.h"
#include "CScrollMgr.h"

CObj::CObj() : m_eDir(DIR_RIGHT), m_eRender(RENDER_END), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_pImgKey(nullptr), m_AttackTime(GetTickCount64())
, m_bSelect(false), m_eObjID(OT_END), m_Map(nullptr), m_pUnitList(nullptr), m_bIsEnemy(false)
	
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_tStat, sizeof(STAT));
	ZeroMemory(&rc, sizeof(STAT));
	ZeroMemory(&info, sizeof(INFO));
}

CObj::~CObj()
{
}

void CObj::Add_Stat_hp(float hp)
{
	m_tStat.m_iHp += hp;

	if (m_tStat.m_iHp < 0.f)
		m_tStat.m_iHp = 0.f;
	if (m_tStat.m_iHp > m_tStat.m_iMaxHp)
		m_tStat.m_iHp = m_tStat.m_iMaxHp;
};

const RECT* CObj::Get_Scroll_Rect()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	rc = m_tRect;
	rc.left += iScrollX;
	rc.top += iScrollY;
	rc.right += iScrollX;
	rc.bottom += iScrollY;

	return &rc;
}

RECT* CObj::Get_Scroll_Rect_Line()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	rc = m_tRect;
	rc.left += iScrollX;
	rc.top += iScrollY;
	rc.right += iScrollX;
	rc.bottom += iScrollY;

	return &rc;
}

INFO CObj::Get_Scroll_Info()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	info = m_tInfo;
	info.fX -= iScrollX;
	info.fY -= iScrollY;

	return info;
}

const INFO* CObj::Get_Scroll_Info_Pointer()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	info = m_tInfo;
	info.fX -= iScrollX;
	info.fY -= iScrollY;

	return &info;
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

DIRECTION CObj::GetDirection(float player_x, float player_y, float monster_x, float monster_y)
{
	float angle_deg = GetLadanAngle(player_x, player_y, monster_x, monster_y) * (180 / PI);

	if (angle_deg < 0) {
		angle_deg += 360;  // 음수를 0-360 범위로
	}

	int sector = static_cast<int>((angle_deg + 11.25) / 22.5) % 16;
	switch (sector) {
	case 0: return DIR_RIGHT;
	case 1: return DIR_RIGHT_DOWN;
	case 2: return DIR_RD;
	case 3: return DIR_DOWN_RIGHT;
	case 4: return DIR_DOWN;
	case 5: return DIR_DOWN_LEFT;
	case 6: return DIR_LD;
	case 7: return DIR_LEFT_DOWN;
	case 8: return DIR_LEFT;
	case 9: return DIR_LEFT_UP;
	case 10: return DIR_LU;
	case 11: return DIR_UP_LEFT;
	case 12: return DIR_UP;
	case 13: return DIR_UP_RIGHT;
	case 14: return DIR_RIGHT_UP;
	case 15: return DIR_RU;
	default: return DIR_END;  // 예외 처리
	}
}

DIRECTION CObj::GetDirectionBattle(float player_x, float player_y, float monster_x, float monster_y)
{
	float angle_deg = GetLadanAngle(player_x, player_y, monster_x, monster_y) * (180 / PI);

	if (angle_deg < 0) {
		angle_deg += 360;  // 음수를 0-360 범위로
	}

	int sector = static_cast<int>((angle_deg + 11.25) / 22.5) % 16;
	switch (sector) {
	case 0: return DIR_RIGHT;
	case 15: return DIR_RIGHT_DOWN;
	case 14: return DIR_RD;
	case 13: return DIR_DOWN_RIGHT;
	case 12: return DIR_DOWN;
	case 11: return DIR_DOWN_LEFT;
	case 10: return DIR_LD;
	case 1: return DIR_LEFT_DOWN;
	case 2: return DIR_LEFT;
	case 3: return DIR_LEFT_UP;
	case 4: return DIR_LU;
	case 5: return DIR_UP_LEFT;
	case 6: return DIR_UP;
	case 7: return DIR_UP_RIGHT;
	case 8: return DIR_RIGHT_UP;
	case 9: return DIR_RU;
	default: return DIR_END;  // 예외 처리
	}
}


float CObj::GetLadanAngle(float player_x, float player_y, float monster_x, float monster_y)
{
	float dx = monster_x - player_x;
	float dy = monster_y - player_y;

	return atan2(dy, dx);  // 라디안
}
