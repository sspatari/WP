// lab4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab4.h"

#define MAX_LOADSTRING 100
#define MY_TIMER 1

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
vector<Figure> figures;							// all figures(either circles or squares)

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void createFigure(HWND, int type = CIRCLE, int x = -1, int y = -1);
int checkFigureCollide(Figure *);
void onPaint(HDC);
void update(HWND);
void collideWindowRect(Figure *, HWND);
void changeSpeed(bool);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	srand(time(NULL));
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcMem;
	RECT rect;
	HBITMAP hbmMem;
	int width;
	int height;
	HBITMAP hOld;

    switch (message)
    {
	case WM_CREATE: {
		SetTimer(hWnd, MY_TIMER, 25, (TIMERPROC)NULL);
 		for (int i = 0; i < 3; i++) {
			createFigure(hWnd); //create 3 circles
		}
		break;
	}

	case WM_TIMER:
		switch (wParam)
		{
		case MY_TIMER:
			update(hWnd);
			break;
		}
		break;

	case WM_MOUSEWHEEL: {
		int zDelta = (short)HIWORD(wParam);
		if (zDelta > 0)
			changeSpeed(true);
		else
			changeSpeed(false);
		break;
	}

	case WM_PAINT:
	{
		GetClientRect(hWnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		hdc = BeginPaint(hWnd, &ps);

		// Create an off-screen DC for double-buffering
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, width, height);
		hOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

		// Fills the Bkg with white color
		FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// Draw into hdcMem here
		onPaint(hdcMem);

		// Transfer the off-screen DC to the screen
		BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_LBUTTONDOWN: 
		createFigure(hWnd, rand() % 2, LOWORD(lParam), HIWORD(lParam));
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
 
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void createFigure(HWND hWnd, int type, int x, int y)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	Figure figure(50 + rand() % (rect.right - rect.left - 100), //sets x coordinates
		50 + rand() % (rect.bottom - rect.top - 100),			//sets y coordinates
		10 + rand() % 10);										//sets radius
	figure.setRandomColor();
	figure.setRandomVelocity();	
	if (type == CIRCLE)
		figure.setCircle();
	else if(type == SQUARE)
		figure.setSquare();

	if (x != -1 && y != -1)
	{
		figure.setPosition(x, y);
	}
	figures.push_back(figure);
}

int checkFigureCollide(Figure *figure)
{
	int count = 0;
	for (vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it) {
		Figure *second = &(*it);
		if (figure != second)
			if (figure->collides(*second)) {
				if (figure->getX() < second->getX()) {
					figure->invertVelocityX();
				}
				if (figure->getY() < second->getY()) {
					figure->invertVelocityY();
				}
				figure->updatePosition();

				if (figure->getType() == second->getType())
				{
					figure->setRandomColor();
					second->setRandomColor();
					count++;
				}
			}
	}

	return count;
}

//paint all figures in the device context
void onPaint(HDC hdc)
{
	for (vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it) {
		Figure figure = *it;
		figure.paint(hdc);
	}
}

//update the hall client rect
void update(HWND hWnd)
{

	RECT rect;
	GetClientRect(hWnd, &rect);
	//used for debuging
	//wchar_t str[256]; 
	//wsprintf(str, l"figures size: %d \n", figures.size());
	//OutputDebugString(str);
	// delete if not necesarry
	int count = 0;

	for (vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it) {
		Figure *figure = &(*it);
		figure->updatePosition();
		collideWindowRect(figure, hWnd);
		count += checkFigureCollide(figure); //counts the collisions
	}
	for (int i = 0; i < count; i++) //for each collision creates a figure
	{
		if (figures.size() < 20) //no more then 20 elements created with collisions
			createFigure(hWnd, rand() % 2);
	}
	InvalidateRect(hWnd, &rect, false); //the false arg is needed to eliminate some of the flickering
}

void collideWindowRect(Figure *figure, HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	if (figure->collidesHorizontalBorder(0) || figure->collidesHorizontalBorder(rect.bottom - rect.top)) {
		figure->invertVelocityY();
		figure->increaseVelocity();
	}
	if (figure->collidesVerticalBorder(0) || figure->collidesVerticalBorder(rect.right - rect.left)) {
		figure->invertVelocityX();
		figure->flipFigure();
	}
}

void changeSpeed(bool increase)
{
	for (vector<Figure>::iterator it = figures.begin(); it != figures.end(); ++it) {
		Figure *figure = &(*it);
		if (increase)
			figure->increaseVelocity();
		else
			figure->decreaseVelocity();
	}
}