#include <windows.h>
#include <time.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SineWave");
	HWND hwnd;
	HACCEL hAccel;
	MSG msg;
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
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("Laboratory 3"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(101));
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
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	static RECT rect;
	static HPEN pens[5];
	int color_r, color_g, color_b;
	static POINT bezier1[4], bezier2[4];
	static POINT polygon[3];
	static POINT circleCenter, squareCenter;
	static RECT circleRect, square;
	static int rePaint = 0;
	static HDC memoryHDC;
	static HBITMAP bitmapMem;
	static HINSTANCE hInstance;
	BITMAP bitmap;
	static TCHAR quote[] = TEXT("Do. Or do not. There is no try.");
	SIZE  lp_Size;
	static HBRUSH selectbrush = NULL;
	static HPEN circlePen;
	static HBRUSH squareBrush;
	static int changeCirclePen = 0, changeSquareBrush = 0, drawBezier = 0, delta = 0, erase = 0;
	LOGPEN logpen;

	srand(time(NULL));

	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		
		// generating random colors for the figures
		for (i = 0; i < 5; i++)
		{
			color_r = rand() % 256;
			color_g = rand() % 256;
			color_b = rand() % 256;
			pens[i] = CreatePen(PS_SOLID, (i + 1) * 5 + (delta / 5), RGB(color_r, color_g, color_b));
		}
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hdc = GetDC(hwnd);
		memoryHDC = CreateCompatibleDC(hdc);
		bitmapMem = LoadBitmap(hInstance, MAKEINTRESOURCE(103));
		SelectObject(memoryHDC, bitmapMem);
		GetObject(bitmapMem, sizeof(bitmap), &bitmap);
		GetTextExtentPoint32(memoryHDC, quote, strlen(quote), &lp_Size);
		SetTextColor(memoryHDC, RGB(255, 255, 255));
		SetBkColor(memoryHDC, RGB(0, 0, 0));
		TextOut(memoryHDC, (bitmap.bmWidth - lp_Size.cx) / 2, (bitmap.bmHeight - lp_Size.cy) / 2, quote, strlen(quote));
		SetScrollRange(hwnd, SB_VERT, 0, 10, TRUE);
		SetScrollPos(hwnd, SB_VERT, 0, TRUE);
		ReleaseDC(hwnd, hdc);
		circlePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		squareBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}