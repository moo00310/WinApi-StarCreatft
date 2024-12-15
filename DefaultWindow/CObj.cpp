#include "pch.h"
#include "CObj.h"

CObj::CObj() : m_eDir(DIR_END), m_eRender(RENDER_END), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_pImgKey(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_tStat, sizeof(STAT));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iCurCount;

		if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
			m_tFrame.iCurCount = m_tFrame.iFrameStart;
			
		m_tFrame.dwTime = GetTickCount64();
	}
}
