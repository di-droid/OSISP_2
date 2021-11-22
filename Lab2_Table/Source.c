#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

HBRUSH hbrPoint;
RECT rect;

int windowWidth;
int windowHeight;

int rectWidth;
int rectHeight;

int numCol = 5;
int numRow = 5;


int letterHeight = 23;

RECT place;



void DrawTable(HDC hdcDest)
{
	SaveDC(hdcDest);

	int j;
	for (int i = 0; i < numCol; i++)
	{
		MoveToEx(hdcDest, i * (windowWidth / numCol), 0, NULL);
		LineTo(hdcDest, i * (windowWidth / numCol), windowHeight);
	}

	for (int i = 0; i < numRow; i++)
	{
		MoveToEx(hdcDest, 0, i * (windowHeight / numRow), NULL);
		LineTo(hdcDest, windowWidth, i * (windowHeight / numRow));
	}

	SelectObject(hdcDest, hbrPoint);

	RestoreDC(hdcDest, -1);
}

void DrawTxt(HDC hdcDest, int i, int j, char txt[])
{
	SaveDC(hdcDest);


	RECT rect;
	rect.left = i * rectWidth + 2;
	rect.top = j * rectHeight + 1;
	rect.right = (i+1) * rectWidth - 1;
	rect.bottom = (j+1) * rectHeight - 1;

	HFONT font;
	font = CreateFont(letterHeight, 0, 0, 0, FW_DONTCARE, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
	SelectObject(hdcDest, font);
	DrawTextA(hdcDest, txt, -1, &rect, DT_WORDBREAK | DT_NOCLIP);

	RestoreDC(hdcDest, -1);
}

LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (uMsg)
	{
	case WM_CREATE:
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;

		rectWidth = windowWidth / numCol;
		rectHeight = windowHeight / numRow;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case (0xBB):
			letterHeight++;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case (0xBD):
			letterHeight--;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);
		DrawTable(hDC);
		DrawTxt(hDC, 0, 1, "Hello");
		DrawTxt(hDC, 0, 4, "Diana");
		DrawTxt(hDC, 1, 1, "Diana");
		DrawTxt(hDC, 4, 1, "Pugachova");
		DrawTxt(hDC, 4, 2, "Have a nice day");
		DrawTxt(hDC, 3, 4, "Don't smoke once a day. Smoke two times or even more");
		EndPaint(hWnd, &ps);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = MyWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyWindowClass";
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);


	HWND hWnd;

	hWnd = CreateWindowEx(0, L"MyWindowClass", L"Table", (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~(WS_MAXIMIZEBOX), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, NULL);

	if (hWnd == INVALID_HANDLE_VALUE) {
		return EXIT_FAILURE;
	}

	
	GetClientRect(hWnd, &rect);
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;

	rectWidth = windowWidth / numCol;
	rectHeight = windowHeight / numRow;

	hbrPoint = CreateSolidBrush(0x007dff);



	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}
	DeleteObject(hbrPoint);
	return msg.wParam;
}