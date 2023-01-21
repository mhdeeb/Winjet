#include "window.h"

void RegisterWindowClass(HINSTANCE hInstance, LPCWSTR className, WNDPROC wndProc, UINT style) {
	WNDCLASS wndclass{};

	wndclass.style = style;
	wndclass.lpfnWndProc = wndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(nullptr, IDC_HAND);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = className;

	RegisterClass(&wndclass);
}

HWND CreateWidgetWindow(LPCWSTR className, HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName, UINT styles, UINT ExStyles, HWND parent) {
	HWND hwnd = CreateWindowEx(
		ExStyles,
		className,
		windowName,
		styles,
		x,
		y,
		width,
		height,
		parent, nullptr, hInstance, nullptr
	);
	return hwnd;
}

void move(HWND hwnd, LONG x, LONG y, HWND insertAfter) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 0);
}