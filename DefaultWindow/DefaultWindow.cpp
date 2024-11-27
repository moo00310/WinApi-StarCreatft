// DefaultWindow.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DefaultWindow.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);   // 창의 외형적인 스타일을 지정하는 옵션 함수
BOOL                InitInstance(HINSTANCE, int);           // 창 초기화 함수
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    // 윈도우 메시지 처리기 함수
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 현재 프로그램의 고유 식별 번호
    _In_opt_ HINSTANCE hPrevInstance,  // 전에 실행되었던 인스턴스의 핸들(없을 경우 NULL)
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)           // 창의 스타일(모양 / 최소화 또는 최대한 모양)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    ///////////////////////////////////////////////////////////////////////////////////////////////
    MSG msg;
    msg.message = WM_NULL;

    MainGame MainGame;
    MainGame.Initialize();

    ULONG64       dwTime = GetTickCount64();

    while (true)
    {
        // PeekMessage : 시스템 메세지 큐로부터 메세지를 읽어오면 TRUE, 읽어올 메세지가 없을 경우 FALSE

        // PM_REMOVE       : 메세지를 읽어옴과 동시에 메세지 제거
        // PM_NOREMOVE     : 메세지 큐에 메세지가 있는지 파악, 메세지가 있을 경우, GetMessage를 호출하여 true처리

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            // 메뉴 기능의 단축키가 제대로 작동하도록 검사하는 함수
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg); // 키보드 메세지를 가공하여 프로그램에서 쉽게 사용할 수 있도록 하는 함수 
                DispatchMessage(&msg); // 시스템 메세지 큐에서 꺼낸 메세지를 프로그램의 메세지 처리기에게 전달하는 함수       
            }
        }
        else
        {
            if (dwTime + 20 < GetTickCount64())
            {
                MainGame.Update();
                MainGame.Render();

                dwTime = GetTickCount64();
            }

        }
    }
    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // WNDCLASSEXW : 창의 여러 특성을 정의하기 위한 구조체
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    // 자기 자신의 사이즈를 저장

    // 초기화 되는 값은 윈도우 창의 수직, 수평의 크기가 변할 경우 다시 그리겠다는 의미
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // CS_HREDRAW : 가로를 다시 그림
    // CS_VREDRAW : 세로를 다시 그림

    // 저장해둔 함수가 호출되어 메세지를 처리
    wcex.lpfnWndProc    = WndProc;

    // 윈도우가 특수한 목적으로 사용하는 여분의 공간(일종의 예약 영역, 사용하지 않을 경우 0)
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    // 이 윈도우를 사용하는 프로그램의 번호를 설정
    wcex.hInstance      = hInstance;

    // 윈도우가 사용할 아이콘 지정
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));

    // 마우스 커서 지정
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    
    // 배경 색을 설정
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);

    // 창의 메뉴바
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
   
    // 실행 파일의 이름(프로젝트 이름과 일치시켜 작성)
    wcex.lpszClassName  = szWindowClass;

    // 윈도우 상단에 있는 아이콘 모양
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc{ 0, 0, WINCX, WINCY };

   // rc = rc + 기본 창 스타일을 고려한 크기 + 메뉴 바 크기 고려 여부
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass,         // 정의한 윈도우 클래스의 전달할 클래스 이름
                             szTitle,               // 창 타이틀 문자열
                             WS_OVERLAPPEDWINDOW,   // 만들고자 하는 윈도우의 형태(스타일)지정, 기본 값으로 흔히 접하는 모양
                             CW_USEDEFAULT, 0,      // 창을 생성하는 X,Y 좌표
                             WINCX, WINCY,          // 생성할 창의 가로, 세로 사이즈
                             nullptr,               // 부모 윈도우의 핸들을 지정, 없으면 null
                             nullptr,               // 윈도우에서 사용할 메뉴의 핸들
                             hInstance,             // 윈도우를 만드는 주체, 프로그램의 핸들 지정
                             nullptr);              // 운영체제가 특수한 목적으로 사용

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}

RECT        rc{ 500, 500, 600, 600 };
list<RECT>  BulletList;
list<RECT> RightBullet;
list<RECT> LeftBullet;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(g_hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


