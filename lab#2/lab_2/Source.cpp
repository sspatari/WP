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
	BS_DEFPUSHBUTTON, TEXT("Press to enter Name"),
	BS_DEFPUSHBUTTON, TEXT("Change font color"),
	BS_DEFPUSHBUTTON, TEXT("Change background color"),
};


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnterNameDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ColorBkgDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ColorTextDlgProc(HWND, UINT, WPARAM, LPARAM);
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
	static HWND hwndColorScroll = NULL, 
		hwndEnterTextDialog = NULL, 
		hwndColorTextDialog = NULL, 
		hwndColorBkgDialog = NULL, 
		hwndEditText = NULL;
	static HDC hdc;
	PAINTSTRUCT ps;
	static int cxChar, cyChar;
	RECT rect;
	int i;
	static char helloMessage[256] = "Your Text Here";
	static int color[3] = { 0, 0, 0 };
	static HBRUSH hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	static char str[100];
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
			(rect.right - strlen(helloMessage) * cxChar) / 2,
			cyChar,
			strlen(helloMessage) * cxChar,
			cyChar,
			hwnd, (HMENU)ID_TEXT,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);

		SetWindowText(hwndText, helloMessage);
		FillListBox(hwndList);
		return 0;
	
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hwndList,
			(rect.right - 20 * cxChar) / 2,
			rect.bottom / 2 - cyChar * 11,
			cxChar * 18 + GetSystemMetrics(SM_CXVSCROLL),
			cyChar * 10,
			TRUE);
		MoveWindow(hwndText,
			(rect.right - strlen(helloMessage) * cxChar) / 2,
			rect.bottom / 2 - cyChar * 15,
			strlen(helloMessage) * cxChar,
			cyChar,
			TRUE);

		for (i = 0; i < 3; i++) {
			GetClientRect(hwnd, &rect);
			MoveWindow(hwndButton[i],
				(rect.right - 30 * cxChar) / 2,
				(rect.bottom / 2) + cyChar * (1 + 3 * i),
				30 * cxChar,
				7 * cyChar / 4,
				TRUE);
		}
		return 0;
	case WM_PAINT:
		if (hwndColorTextDialog != NULL)
		{
			for (i = 10; i < 13; i++)
			{
				hwndColorScroll = GetDlgItem(hwndColorTextDialog, i);
				color[i - 10] = GetScrollPos(hwndColorScroll, SB_CTL);
			}
		}
		hdc = BeginPaint(hwnd, &ps);
		SetTextColor(hdc, RGB(color[0], color[1], color[2]));
		SetWindowText(hwndText, helloMessage);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_CTLCOLORSTATIC: //needed in order to change color text of the our helloMessage text
	{
		SetBkMode((HDC)wParam, TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(color[0], color[1], color[2])
		);
		return (LRESULT)hbr;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{ 
		case 0:
			hwndEnterTextDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, EnterNameDlgProc); 
			break;

		case 1:
			hwndColorTextDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, ColorTextDlgProc);
			break;

		case 2:
			hwndColorBkgDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, ColorBkgDlgProc);
			break;
			
		case 3:
			GetClientRect(hwnd, &rect);
			strcpy_s(helloMessage, "Hi, ");
			hwndEditText = GetDlgItem(hwndEnterTextDialog, IDC_EDIT1);
			GetWindowText(hwndEditText, str, 30);
			OutputDebugString(str);
			strcat_s(str, " died in attempt of learning WIN API");
			strcpy_s(helloMessage, str);
			EndDialog(hwndEnterTextDialog, 0);
			MoveWindow(hwndText,
				(rect.right - strlen(helloMessage) * cxChar) / 2,
				rect.bottom / 2 - cyChar * 15,
				strlen(helloMessage) * cxChar,
				cyChar,
				TRUE);
			SetWindowText(hwndText, helloMessage);
			break;

		case ID_LIST:
			GetClientRect(hwnd, &rect);
			i = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			SendMessage(hwndList, LB_GETTEXT, i,(LPARAM)str);
			strcpy_s(helloMessage, sizeof(helloMessage), "You selected ");
			strcat_s(helloMessage, str);
			OutputDebugString(helloMessage);
			MoveWindow(hwndText,
				(rect.right - strlen(helloMessage) * cxChar) / 2,
				rect.bottom / 2 - cyChar * 15,
				strlen(helloMessage) * cxChar,
				cyChar,
				TRUE);
			SetWindowText(hwndText, helloMessage);
			break;

		case ID_ACCELERATOR40004:
			GetWindowRect(hwnd, &rect);
			rect.left += 20;
			rect.right -= 20;
			MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, TRUE);
			break;

		case ID_ACCELERATOR40005:
			GetWindowRect(hwnd, &rect);
			rect.left -= 20;
			rect.right += 20;
			MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, TRUE);
			break;
		case ID_MENU_ABOUT:
			MessageBox(hwnd, TEXT("About Menu\n")
				TEXT("\nFeel the pain"), TEXT("LAB2"), MB_OK | MB_ICONINFORMATION);
			break;

		case ID_MENU_HELP:
			MessageBox(hwnd, TEXT("Help Menu\n")
				TEXT("\nHelp cannot be provided :D"), TEXT("LAB2"), MB_OK | MB_ICONEXCLAMATION);
			break;

		case ID_MENU_DELETEMENU:
			SetMenu(hwnd, NULL);
			break;
		}
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
		sprintf_s(str, "List element #%d", i + 1);
		SendMessage(hwndList, LB_INSERTSTRING, i, (LPARAM)str);
	}
}

BOOL CALLBACK EnterNameDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{ 
	switch (message) 
	{ case WM_INITDIALOG: 
		return TRUE; 
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			SendMessage(GetParent(hDlg), WM_COMMAND, 3, lParam);
		case IDCANCEL: 
			EndDialog(hDlg, 0); 
			return TRUE; 
		} 
		break;
	}
	return FALSE; 
}

BOOL CALLBACK ColorBkgDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iColor[3];
	HWND hwndParent, hCtrl;
	int iCtrlID, iIndex;
	switch (message)
	{
	case WM_INITDIALOG:
		for (iCtrlID = 10; iCtrlID < 13; iCtrlID++)
		{
			hCtrl = GetDlgItem(hDlg, iCtrlID);
			SetScrollRange(hCtrl, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hCtrl, SB_CTL, 0, FALSE);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	case WM_HSCROLL:
		hCtrl = (HWND)lParam;
		iCtrlID = GetWindowLong(hCtrl, GWL_ID);
		iIndex = iCtrlID - 10;
		hwndParent = GetParent(hDlg);
		switch (LOWORD(wParam))
		{
		case SB_PAGEDOWN:
			iColor[iIndex] += 15; // fall through
		case SB_LINEDOWN:
			iColor[iIndex] = min(255, iColor[iIndex] + 1);
			break;
		case SB_PAGEUP:
			iColor[iIndex] -= 15; // fall through
		case SB_LINEUP:
			iColor[iIndex] = max(0, iColor[iIndex] - 1);
			break;
		case SB_TOP:
			iColor[iIndex] = 0;
			break;
		case SB_BOTTOM:
			iColor[iIndex] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iColor[iIndex] = HIWORD(wParam);
			break;
		default:
			return FALSE;
		}
		SetScrollPos(hCtrl, SB_CTL, iColor[iIndex], TRUE);
		SetDlgItemInt(hDlg, iCtrlID + 3, iColor[iIndex], FALSE);
		DeleteObject((HGDIOBJ)SetClassLong(hwndParent, GCL_HBRBACKGROUND,
			(LONG)CreateSolidBrush(RGB(iColor[0], iColor[1], iColor[2]))));
		InvalidateRect(hwndParent, NULL, TRUE);
		return TRUE;
	}
	return FALSE;
}


BOOL CALLBACK ColorTextDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iColor[3];
	HWND hwndParent, hCtrl;
	int iCtrlID, iIndex;
	switch (message)
	{
	case WM_INITDIALOG:
		for (iCtrlID = 10; iCtrlID < 13; iCtrlID++)
		{
			hCtrl = GetDlgItem(hDlg, iCtrlID);
			SetScrollRange(hCtrl, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hCtrl, SB_CTL, 0, FALSE);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	case WM_HSCROLL:
		hCtrl = (HWND)lParam;
		iCtrlID = GetWindowLong(hCtrl, GWL_ID);
		iIndex = iCtrlID - 10;
		hwndParent = GetParent(hDlg);
		switch (LOWORD(wParam))
		{
		case SB_PAGEDOWN:
			iColor[iIndex] += 15; // fall through
		case SB_LINEDOWN:
			iColor[iIndex] = min(255, iColor[iIndex] + 1);
			break;
		case SB_PAGEUP:
			iColor[iIndex] -= 15; // fall through
		case SB_LINEUP:
			iColor[iIndex] = max(0, iColor[iIndex] - 1);
			break;
		case SB_TOP:
			iColor[iIndex] = 0;
			break;
		case SB_BOTTOM:
			iColor[iIndex] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iColor[iIndex] = HIWORD(wParam);
			break;
		default:
			return FALSE;
		}
		SetScrollPos(hCtrl, SB_CTL, iColor[iIndex], TRUE);
		SetDlgItemInt(hDlg, iCtrlID + 3, iColor[iIndex], FALSE);
		InvalidateRect(hwndParent, NULL, TRUE);
		return TRUE;
	}
	return FALSE;
}