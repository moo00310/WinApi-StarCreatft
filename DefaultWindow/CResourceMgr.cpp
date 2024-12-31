#include "pch.h"
#include "CResourceMgr.h"

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

void CResourceMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Resource.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);

	for (auto& resource : m_vecResource)
	{
		WriteFile(hFile, resource->Get_Info_Pointer(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"¼º°ø", MB_OK);

}

void CResourceMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Resource.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	INFO	tResource{};

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &tResource, sizeof(INFO), &dwByte, NULL);

		if (0 == dwByte)
			break;

		//CResource* pResource = CAbstractFactory<CTile>::Create(tTile.fX, tTile.fY);
		//m_vecResource.push_back(pResource);
	}

	CloseHandle(hFile);
}
