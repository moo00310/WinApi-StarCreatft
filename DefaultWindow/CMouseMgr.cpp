#include "pch.h"
#include "CMouseMgr.h"

CMouseMgr* CMouseMgr::m_pInstance = nullptr;



void CMouseMgr::Add_Mouse(CObj* _mouse)
{
	m_pMouse = _mouse;
}

void CMouseMgr::Update()
{
	if(m_pMouse)
		m_pMouse->Update();
}

void CMouseMgr::Late_Update()
{
	if (m_pMouse)
		m_pMouse->Late_Update();
}

void CMouseMgr::Render(HDC hDC)
{
	if (m_pMouse)
		m_pMouse->Render(hDC);
}

void CMouseMgr::Release()
{
	Safe_Delete(m_pMouse);
}

