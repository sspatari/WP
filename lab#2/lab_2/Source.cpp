#include <windows.h>
#include <stdio.h>
#include "resource.h"
#define _UNICODE

#define IDM_SYS_ABOUT 1
#define IDM_SYS_HELP 2
#define IDM_SYS_REMOVE 3
#define ID_LIST 30
#define ID_TEXT 40

struct
{
	int iStyle;
	TCHAR * szText;
}
button[] =
{
	BS_DEFPUSHBUTTON, TEXT("Press Me!"),
	BS_DEFPUSHBUTTON, TEXT("Change font color"),
	BS_DEFPUSHBUTTON, TEXT("Change background color"),
};


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void FillListBox(HWND hwndList);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("LAB2");
	HACCEL hAccel;
	HMENU hMenu, hSysMenu;
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON)LoadImage(hInstance, "chemistry.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	wndclass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	
	//Obtainint handle to main menu
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	hwnd = CreateWindow(szAppName,
		TEXT("Laboratory Nr.2"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		NULL, 
		hMenu,
		hInstance,
		NULL); 

	//Add menus to system menu
	hSysMenu = GetSystemMenu(hwnd, FALSE);
	AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hSysMenu, MF_STRING, IDM_SYS_ABOUT, TEXT("About..."));
	AppendMenu(hSysMenu, MF_STRING, IDM_SYS_HELP, TEXT("Help..."));
	AppendMenu(hSysMenu, MF_STRING, IDM_SYS_REMOVE, TEXT("Remove Additions"));

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	//load accelerator table
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndButton[3];
	static HINSTANCE hInstance;
	static HWND hwndList, hwndText;
	static int cxChar, cyChar;
	RECT rect;
	int i;



	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		for (i = 0; i < 3; i++) {
			hwndButton[i] = CreateWindow(TEXT("button"),
				button[i].szText,
				WS_CHILD | WS_VISIBLE | button[i].iStyle,
				(rect.right - 30 * cxChar) / 2,
				(rect.bottom / 2) + cyChar * (1 + 3 * i),
				30 * cxChar,
				7 * cyChar / 4,
				hwnd, (HMENU)i,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hwndList = CreateWindow(TEXT("listbox"), NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD,
			(rect.right - 20 * cxChar) / 2,
			cyChar * 10,
			cxChar * 18 + GetSystemMetrics(SM_CXVSCROLL),
			cyChar * 10,
			hwnd, (HMENU)ID_LIST,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);
		hwndText = CreateWindow(TEXT("static"), NULL,
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			cxChar, cyChar,
			GetSystemMetrics(SM_CXSCREEN), cyChar,
			hwnd, (HMENU)ID_TEXT,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);
		FillListBox(hwndList);
		return 0;
	case WM_SYSCOMMAND:   //system menu 
		switch (LOWORD(wParam)) {
		case IDM_SYS_ABOUT:              
			MessageBox(hwnd, TEXT("About System Menu\n")                                 
				TEXT("\nFeel the pain"), TEXT("LAB2"), MB_OK | MB_ICONINFORMATION);
			return 0;
		case IDM_SYS_HELP:               
			MessageBox(hwnd, TEXT("Help System Menu\n")
				TEXT("\nHelp cannot be provided :D"), TEXT("LAB2"), MB_OK | MB_ICONEXCLAMATION);
			return 0;
		case IDM_SYS_REMOVE:            
			GetSystemMenu(hwnd, TRUE); // this will remove the added submenus from system menus      
			return 0;
		}      
		break; //break needed in order to pass the obtained default case to DefWindowProc
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void FillListBox(HWND hwndList)
{
	int i;
	TCHAR str[256];
	for (i = 0; i < 15; ++i)
	{
		sprintf_s(str, "List Element #%d", i + 1);
		SendMessage(hwndList, LB_INSERTSTRING, i, (LPARAM)str);
	}
}