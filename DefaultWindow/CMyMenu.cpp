#include "pch.h"
#include "CMyMenu.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"

CMyMenu::CMyMenu():m_pMouse(nullptr)
{
}

CMyMenu::~CMyMenu()
{
    Release();
}

void CMyMenu::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MenuBack.bmp", L"MenuBack");

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/SinglePlay/single0_34(320.116).bmp", L"single");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Editor/editor0_84(160.140).bmp", L"editor");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Exit/exit0_49(184.128).bmp", L"exit");

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/SinglePlay/singleon0_59(252.124).bmp", L"singleon");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Editor/editoron0_19(204.132).bmp", L"editoron");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Exit/exiton0_29(216.136).bmp", L"exiton");

  
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Button/single_button.bmp", L"single_button");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Button/editor_button.bmp", L"editor_button");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/MainMenu/Button/exit_button.bmp", L"exit_button");


    CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CButtonMouse>::Create());

    CObj* pButton = CAbstractFactory<CMyButton>::CreateButton(200.f, 200.f, L"single");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CMyButton>::CreateButton(550.f, 200.f, L"editor");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CMyButton>::CreateButton(550.f, 400.f, L"exit");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    //-------------------------------------------------------------------------------

    pButton = CAbstractFactory<CAnimeButton>::CreateButton(200.f, 270.f, L"singleon");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CAnimeButton>::CreateButton(611.f, 220.f, L"editoron");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    pButton = CAbstractFactory<CAnimeButton>::CreateButton(580.f, 400.f, L"exiton");
    CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

    
}

int CMyMenu::Update()
{
    CObjMgr::Get_Instance()->Update();
    return 0;
}

void CMyMenu::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC hDC)
{
    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MenuBack");

    BitBlt(hDC,
        0, 0, WINCX, WINCY,
        hMemDC,
        0,
        0,
        SRCCOPY);

    CObjMgr::Get_Instance()->Render(hDC);

}

void CMyMenu::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
    CObjMgr::Get_Instance()->Delete_ID(OBJ_MOUSE);

}
