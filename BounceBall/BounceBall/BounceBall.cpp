// BounceBall.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "BounceBall.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOUNCEBALL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNCEBALL));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNCEBALL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOUNCEBALL);
    wcex.lpszClassName  = szWindowClass;
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, 
                            0, 1000, 700, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
Game game;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        if (currentScreen == TITLE_SCREEN)
        {
            CreateWindow(TEXT("button"), TEXT("게임 시작"), WS_VISIBLE | WS_CHILD, 400, 250, 200, 50, hWnd, (HMENU)1001, hInst, NULL);
            CreateWindow(TEXT("button"), TEXT("색 변경하기"), WS_VISIBLE | WS_CHILD, 400, 350, 200, 50, hWnd, (HMENU)1002, hInst, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        
    }
    break;
    case WM_ERASEBKGND:
        return 1;
    case WM_LBUTTONDOWN:
    {
        // 마우스 클릭 위치
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);

        // 비트맵 이미지 영역
        RECT bitmapRect = { 900, 20, 940, 60 };

        if (xPos >= bitmapRect.left && xPos <= bitmapRect.right && yPos >= bitmapRect.top && yPos <= bitmapRect.bottom)
        {
            game.suspendGame();
            CreateWindow(TEXT("button"), TEXT("메인화면"), WS_VISIBLE | WS_CHILD, 400, 230, 200, 50, hWnd, (HMENU)3001, hInst, NULL);
            CreateWindow(TEXT("button"), TEXT("다시하기"), WS_VISIBLE | WS_CHILD, 400, 330, 200, 50, hWnd, (HMENU)3002, hInst, NULL);
            CreateWindow(TEXT("button"), TEXT("계속하기"), WS_VISIBLE | WS_CHILD, 400, 430, 200, 50, hWnd, (HMENU)3003, hInst, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            isLeftPressed = false;
            break;

        case VK_RIGHT:
            isRightPressed = false;
            break;
        }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            isLeftPressed = true;
            game.moveLeft();
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case VK_RIGHT:
            isRightPressed = true;
            game.moveRight();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case 1001:
        {
            currentScreen = GAME_SCREEN;
            DestroyWindow(GetDlgItem(hWnd, 1001));
            DestroyWindow(GetDlgItem(hWnd, 1002));

            game.startGame(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        case 1002:
        {
            currentScreen = COLOR_SCREEN;
            DestroyWindow(GetDlgItem(hWnd, 1001));
            DestroyWindow(GetDlgItem(hWnd, 1002));
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        case 3001:
        {
            currentScreen = TITLE_SCREEN;
            game.stopGame();
            DestroyWindow(GetDlgItem(hWnd, 3001));
            DestroyWindow(GetDlgItem(hWnd, 3002));
            DestroyWindow(GetDlgItem(hWnd, 3003));
            CreateWindow(TEXT("button"), TEXT("게임 시작"), WS_VISIBLE | WS_CHILD, 400, 250, 200, 50, hWnd, (HMENU)1001, hInst, NULL);
            CreateWindow(TEXT("button"), TEXT("색 변경하기"), WS_VISIBLE | WS_CHILD, 400, 350, 200, 50, hWnd, (HMENU)1002, hInst, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        break;
        case 3002:
        {
            DestroyWindow(GetDlgItem(hWnd, 3001));
            DestroyWindow(GetDlgItem(hWnd, 3002));
            DestroyWindow(GetDlgItem(hWnd, 3003));
            game.initBall();
        }
        break;
        case 3003:
        {
            DestroyWindow(GetDlgItem(hWnd, 3001));
            DestroyWindow(GetDlgItem(hWnd, 3002));
            DestroyWindow(GetDlgItem(hWnd, 3003));
            game.resumeGame();
        }
        break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1000, 700);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        HBRUSH bgBrush = CreateSolidBrush(RGB(200, 245, 255));
        FillRect(memDC, &ps.rcPaint, bgBrush);
        DeleteObject(bgBrush);

        if (currentScreen == GAME_SCREEN) 
        {
            game.createGame(hWnd, memDC);

            HDC imgDC = CreateCompatibleDC(memDC);
            HBITMAP MyBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 40, 40, LR_CREATEDIBSECTION);
            HBITMAP oldImgBitmap = (HBITMAP)SelectObject(imgDC, MyBitmap);

            BitBlt(memDC, 900, 20, 40, 40, imgDC, 0, 0, SRCCOPY);

            SelectObject(imgDC, oldImgBitmap);
            DeleteDC(imgDC);
            DeleteObject(hPauseImage);
        }
        else if (currentScreen == COLOR_SCREEN) {
            TextOut(memDC, 10, 10, TEXT("색 변경 화면"), lstrlen(TEXT("색 변경 화면")));
        }
        else if (currentScreen == PAUSE_SCREEN) {
            //TextOut(hdc, 400, 100, TEXT("게임이 일시정지되었습니다."), lstrlen(TEXT("게임이 일시정지되었습니다.")));
        }

        BitBlt(hdc, 0, 0, 1000, 700, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        game.stopGame();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
