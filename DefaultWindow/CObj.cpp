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
