#include "pch.h"
#include "CUIMgr.h"
#include "CMainUI.h"
#include "CAbstractFactory.h"
#include "CPortUI.h"
#include "CWireUI.h"
#include "CWireSmallUI.h"
#include "CSpawnUI.h"
#include "CIconUI.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initalize()
{
	Add_UI(UI_MAIN, CAbstractFactory<CMainUI>::CreateUI());
	Add_UI(UI_MAIN, CAbstractFactory<CPortUI>::CreateUI());
	Add_UI(UI_MAIN, CAbstractFactory<CWireUI>::CreateUI());
	Add_UI(UI_MAIN, CAbstractFactory<CWireSmallUI>::CreateUI());
	Add_UI(UI_MAIN, CAbstractFactory<CSpawnUI>::CreateUI());
	Add_UI(UI_MAIN, CAbstractFactory<CIconUI>::CreateUI());
}

void CUIMgr::Add_UI(UI_TYPE etype, CUI* pUI)
{
	if (UI_END <= etype || nullptr == pUI)
		return;

	m_UIList[etype].push_back(pUI);
}

int CUIMgr::Update()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto iter = m_UIList[i].begin();
			iter != m_UIList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CUI*>(*iter);
				*iter = nullptr;
				iter = m_UIList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CUIMgr::Late_Update()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& pObj : m_UIList[i])
			pObj->Late_Update();
	}
}

void CUIMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& pObj : m_UIList[i])
			pObj->Render(hDC);
	}
}

void CUIMgr::Release()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), Safe_Delete<CUI*>);
		m_UIList[i].clear();
	}
}

