#if defined(_WIN32) && !defined(SDL2)
#include <windows.h>
#include <mmsystem.h>
#include "resource.h"
#include "Screen.hpp"

#pragma comment(lib, "Winmm.lib")

void MainLoop_Setup(void);
void MainLoop_Finalize(void);
bool MainLoop_Loop(void);
void MainLoop_NoWaitLoop(void);

HINSTANCE instanceHandle;
HWND windowHandle;
DWORD windowStyle;
TIMECAPS timeCaps;

// CreateDIBSection
HDC bitmapHDC;
HBITMAP bitmapHandle;
LPBYTE bitmapInfoBuffer;
LPDWORD bitmap;
LPBITMAPINFO bitmapInfo;

int screenScale = 8;

void InitializeCreateDIBSection(HWND windowHandle)
{
	HDC hdc;
	hdc = GetDC(windowHandle);
	bitmapInfoBuffer = reinterpret_cast<LPBYTE>(GlobalAlloc(GPTR, sizeof(BITMAPINFO)));
	bitmapInfo = reinterpret_cast<LPBITMAPINFO>(bitmapInfoBuffer);
	bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth = Screen::WIDTH;
	bitmapInfo->bmiHeader.biHeight = Screen::HEIGHT;
	bitmapInfo->bmiHeader.biPlanes = 1;
	bitmapInfo->bmiHeader.biBitCount = 32;
	bitmapInfo->bmiHeader.biCompression = BI_RGB;
	bitmapInfo->bmiHeader.biSizeImage = 0;
	bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biClrUsed = 0;
	bitmapInfo->bmiHeader.biClrImportant = 0;
	bitmapHandle = CreateDIBSection(hdc, bitmapInfo, DIB_RGB_COLORS, reinterpret_cast<PVOID*>(&bitmap), NULL, 0);
	bitmapHDC = CreateCompatibleDC(hdc);
	SelectObject(bitmapHDC, bitmapHandle);
}

void FinalizeCreateDIBSection()
{
	GlobalFree(bitmapInfoBuffer);
	DeleteDC(bitmapHDC);
	DeleteObject(bitmapHandle);
}

void DrawCreateDIBSection(HDC hdc)
{
	RECT destRect;
	GetClientRect(windowHandle, &destRect);
	StretchBlt(hdc, destRect.left, destRect.top, destRect.right, destRect.bottom, bitmapHDC, 0, 0, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, SRCCOPY);
}

unsigned int* GetFrameBuffer(void)
{
	return reinterpret_cast<unsigned int*>(bitmap);
}

HWND GetWindowHandle(void)
{
	return windowHandle;
}

void SetWindowSize(int screenScale)
{
	int width = Screen::WIDTH * screenScale;
	int height = Screen::HEIGHT * screenScale;
	RECT position;
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	// ウインドウ位置
	GetWindowRect(windowHandle, &position);
	// ウインドウサイズ
	AdjustWindowRect(&rect, windowStyle, TRUE);
	if(rect.left != 0)
	{
		rect.right -= rect.left;
		rect.left = 0;
	}
	if(rect.top != 0)
	{
		rect.bottom -= rect.top;
		rect.top = 0;
	}
	SetWindowPos(windowHandle, HWND_TOP, position.left, position.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(windowHandle);
			break;
		case ID_WINDOWSIZEx1:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_UNCHECKED);
			screenScale = 4;
			SetWindowSize(screenScale);
			break;
		case ID_WINDOWSIZEx2:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_UNCHECKED);
			screenScale = 8;
			SetWindowSize(screenScale);
			break;
		case ID_WINDOWSIZEx3:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_CHECKED);
			screenScale = 12;
			SetWindowSize(screenScale);
			break;
		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(windowHandle, &ps);
		DrawCreateDIBSection(hdc);
		EndPaint(windowHandle, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

void InitializeWindow(HINSTANCE instance, int cmd_show, int width, int height, LPCWSTR window_name, LPCWSTR class_name)
{
	WNDCLASSEXW wcex;
	int window_width;
	int window_height;
	instanceHandle = instance;
	// ウインドウクラスの定義
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, reinterpret_cast<PCTSTR>(IDI_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINMAIN);
	wcex.lpszClassName = class_name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<PCTSTR>(IDI_ICON));
	RegisterClassEx(&wcex);
	// ウインドウのスタイル設定
	windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
	// ウインドウの大きさを補正する
	RECT window_rect = {0, 0, width, height};
	AdjustWindowRect(&window_rect, windowStyle, TRUE);
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;
	// ウインドウ作成
	windowHandle = CreateWindowW(wcex.lpszClassName, window_name, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, nullptr, nullptr, instance, nullptr);
	InitializeCreateDIBSection(windowHandle);
	ShowWindow(windowHandle, cmd_show);
	UpdateWindow(windowHandle);
	CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_CHECKED);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CoInitialize(NULL);
	InitializeWindow(hInstance, nCmdShow, Screen::WIDTH * screenScale, Screen::HEIGHT * screenScale, CONFIG_TITLE, CONFIG_TITLE);
	// タイマ精度設定
	timeGetDevCaps(&timeCaps, sizeof(TIMECAPS));
	timeBeginPeriod(timeCaps.wPeriodMin);
	// メインループ初期化
	MainLoop_Setup();
	// メインループ
	DWORD frameTime = 1000 / Screen::FPS;
	DWORD beforeTime = timeGetTime();
	MSG msg;
	for(;;)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			if(!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			DWORD time = timeGetTime();
			DWORD diffTime = time - beforeTime;
			if(diffTime > frameTime)
			{
				InvalidateRect(windowHandle, NULL, FALSE);
				UpdateWindow(windowHandle);
				bool exit = MainLoop_Loop();
				if(exit == true)
				{
					DestroyWindow(windowHandle);
				}
				beforeTime = time;
			}
			MainLoop_NoWaitLoop();
		}
	}
	// プログラム終了
	MainLoop_Finalize();
	FinalizeCreateDIBSection();
	timeEndPeriod(timeCaps.wPeriodMin);
	CoUninitialize();
	return static_cast<int>(msg.wParam);
}

#endif
