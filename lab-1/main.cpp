#include <windows.h>

#define ID_EDITCHILD 100
#define ID_EDITCHILD2 101
#define BUTTON_ID1 102
#define BUTTON_ID2 103

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow) {
	TCHAR tcAppName[] = TEXT("SampleWindow");
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = tcAppName;

	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			tcAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(
		tcAppName,                  // window class name
		TEXT("Title"),	            // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		650,                        // initial x size
		650,                        // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);						// creation parameters
	
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HFONT hFont;
	LPWSTR editMesage = L"Just a simple message";
	switch (message) {
	case WM_CREATE:
		static HWND hwndEdit;
		hwndEdit = CreateWindowEx(
			0, TEXT("EDIT"),                                     // predefined class 
			NULL,                                           // no window title 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			200, 10, 150, 200,                              // set size in WM_SIZE message 
			hwnd,                                           // parent window 
			(HMENU)ID_EDITCHILD,                            // edit control ID 
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);                                          // pointer not needed 
		hFont = CreateFont(0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Times New Roman"));
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)editMesage);

		static HWND hwndEdit2;
		hwndEdit2 = CreateWindowEx(
			0, TEXT("EDIT"),                                      // predefined class 
			NULL,                                            // no window title 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			400, 10, 150, 200,                               // set size in WM_SIZE message 
			hwnd,                                            // parent window 
			(HMENU)ID_EDITCHILD2,                            // edit control ID 
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);                                           // pointer not needed 
	
		SendMessage(hwndEdit2, WM_SETTEXT, 0, (LPARAM)editMesage);

		static HWND button1 = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Change Window Name"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			10,         // y position 
			180,        // Button width
			50,        // Button height
			hwnd,     // Parent window
			(HMENU)BUTTON_ID1,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		static HWND button2 = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Change Message"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,  // Styles 
			10,         // x position 
			80,         // y position 
			180,        // Button width
			50,        // Button height
			hwnd,     // Parent window
			(HMENU)BUTTON_ID2,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		return 0;

	case WM_CTLCOLOREDIT:
		hdc = (HDC)wParam;
		if (GetDlgItem(hwnd, ID_EDITCHILD) == (HWND)lParam)
		{
			SetBkColor(hdc, RGB(0, 255, 0)); // Set background color to blue
			SetTextColor(hdc, RGB(255, 0, 0)); // Set text color to red
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Done with Pride and Prejudice by Spatari Stanislav"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DRAWITEM:
		LPDRAWITEMSTRUCT Item;
		Item = (LPDRAWITEMSTRUCT)lParam;
		SelectObject(Item->hDC, CreateFont(16, 0, 0, 0, 
			FW_NORMAL, 
			0, 0, 0, 
			DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, 
			DEFAULT_PITCH, 
			TEXT("Arial")));
		FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(RGB(0, 0, 255)));
		SelectObject(Item->hDC, CreateSolidBrush(0));
		if (Item->itemState & ODS_SELECTED)
		{
			SetTextColor(Item->hDC, RGB(0, 255, 0));
			SelectObject(Item->hDC, CreateSolidBrush(RGB(0,4,23)));
			SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, RGB(100, 100, 1)));
		}
		else
		{
			SetTextColor(Item->hDC, RGB(255, 0, 0));
			SelectObject(Item->hDC, CreateSolidBrush(RGB(50, 5, 100)));
			SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, RGB(255, 255, 1)));

		}
		SetBkMode(Item->hDC, TRANSPARENT);
		RoundRect(Item->hDC, Item->rcItem.left, Item->rcItem.top, Item->rcItem.right, Item->rcItem.bottom, 50, 50);
		int len;
		len = GetWindowTextLength(Item->hwndItem);
		LPSTR lpBuff;
		lpBuff = new char[len + 1];
		GetWindowTextA(Item->hwndItem, lpBuff, len + 1);
		DrawTextA(Item->hDC, lpBuff, len, &Item->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
