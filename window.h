#pragma once

#include <Windows.h>

void registerWindow(HINSTANCE hInstance, LPCWSTR className, WNDPROC wndProc, UINT style = CS_HREDRAW | CS_VREDRAW) {
	WNDCLASS wndclass{};

	wndclass.style = style;
	wndclass.lpfnWndProc = wndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = className;

	RegisterClass(&wndclass);
}

HWND CreateWidgetWindow(LPCWSTR className, HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP) {
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

HWND CreateOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, int x, int y, int width, int height, LPCWSTR className = nullptr, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP) {
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

HWND CreateParentOverlayWindow(HINSTANCE hInstance) {
	return CreateOverlayWindow(hInstance, nullptr, 0, 0, 0, 0, L"Parent", WS_MAXIMIZE, WS_EX_TRANSPARENT, HWND_DESKTOP);
}

HWND CreateChildOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, LPCWSTR className, HWND parent, int x, int y, int width, int height, WNDPROC wndProc = nullptr, UINT styles = NULL) {
	if (!GetClassInfoEx(hInstance, className, nullptr)) {
		registerWindow(hInstance, className, wndProc);
	}
	return CreateOverlayWindow(hInstance, windowName, x, y, width, height, className, WS_CHILDWINDOW | WS_VISIBLE | styles, NULL, parent);
}

void move(HWND hwnd, LONG x, LONG y, HWND insertAfter = nullptr) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 0);
}
