#include <windows.h>
#include <stdio.h>
#include "resource.h"
#define _UNICODE

#define IDM_SYS_ABOUT 1
#define IDM_SYS_HELP 2
#define IDM_SYS_REMOVE 3

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
	RECT rect;

	switch (message)
	{
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
			GetSystemMenu(hwnd, TRUE);           
			return 0;
		}      
		break; //break needed in order to pass the obtained default case to DefWindowProc
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}