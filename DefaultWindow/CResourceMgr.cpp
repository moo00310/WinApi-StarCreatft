#include "pch.h"
#include "CResourceMgr.h"
#include "CAbstractFactory.h"

CResourceMgr* CResourceMgr::m_pInstance = nullptr;

void CResourceMgr::Update()
{
	for (auto& resource : m_vecResource)
	{
		resource->Update();
	}
}

void CResourceMgr::Render(HDC hDC)
{
	for (auto& resource : m_vecResource)
	{
		resource->Render(hDC);
	}
}

void CResourceMgr::Release()
{
	for_each(m_vecResource.begin(), m_vecResource.end(), Safe_Delete<CResource*>);
	m_vecResource.clear();
	m_vecResource.shrink_to_fit();
}

void CResourceMgr::Save_Resorce()
{
	HANDLE hFile = CreateFile(L"../Data/Resource.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	int		iOption(0);

	for (auto& resource : m_vecResource)
	{
		iOption = resource->GetOption();

		WriteFile(hFile, resource->Get_Info_Pointer(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Resource Save", L"¼º°ø", MB_OK);

}

void CResourceMgr::Load_Resorce()
{
	HANDLE hFile = CreateFile(L"../Data/Resource.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	INFO	tResource{};
	int		iOption(0);

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &tResource, sizeof(INFO), &dwByte, NULL);
		a = ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
		if (0 == dwByte)
			break;

		CResource* pResource = CAbstractFactory<CResource>::CreateResource((int)tResource.fX, (int)tResource.fY, iOption);
		m_vecResource.push_back(pResource);
	}

	CloseHandle(hFile);
}
