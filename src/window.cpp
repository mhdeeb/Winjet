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

HWND CreateOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, int x, int y, int width, int height, LPCWSTR className, UINT styles, UINT ExStyles, HWND parent) {
	HWND hwnd = CreateWidgetWindow(className,
		hInstance,
		x, y, width, height,
		windowName,
		WS_POPUP | styles,
		WS_EX_LAYERED | ExStyles,
		parent);
	SetLayeredWindowAttributes(hwnd, TRANSPARENT, NULL, LWA_COLORKEY);
	return hwnd;
}

HWND CreateHiddenWindow(HINSTANCE hInstance) {
	HWND hwnd = CreateWidgetWindow(HIDDENCLASS,
		hInstance,
		0, 0, 0, 0,
		nullptr,
		WS_POPUP,
		WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
		HWND_DESKTOP);
	return hwnd;
}

HWND CreateCanvasOverlayWindow(HINSTANCE hInstance, HWND parent) {
	return CreateOverlayWindow(hInstance, nullptr, 0, 0, 0, 0, CANVASCLASS, WS_MAXIMIZE | WS_VISIBLE, WS_EX_TRANSPARENT, parent);
}

HWND CreateChildOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, LPCWSTR className, HWND parent, int x, int y, int width, int height, UINT styles) {
	return CreateOverlayWindow(hInstance, windowName, x, y, width, height, className, WS_CHILDWINDOW | WS_VISIBLE | styles, NULL, parent);
}

void move(HWND hwnd, LONG x, LONG y, HWND insertAfter) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 0);
}