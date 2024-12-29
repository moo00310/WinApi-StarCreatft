#include "pch.h"
#include "CGameMgr.h"

CGameMgr* CGameMgr::m_pInstance = nullptr;

void CGameMgr::Initialize()
{
	m_iMineal = 0;
	m_iGas = 0;
	m_iPop = 0;
}

void CGameMgr::Update()
{
}

void CGameMgr::Late_Update()
{
}

void CGameMgr::Render(HDC hDC)
{

}

void CGameMgr::Release()
{
}
