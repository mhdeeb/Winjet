#pragma once

#include <Windows.h>

struct window {
	HWND hwnd;
	HINSTANCE hInstance;
	RECT rect;

	window(HWND hwnd, HINSTANCE hInstance): hwnd(hwnd), hInstance(hInstance) {
		GetWindowRect(hwnd, &rect);
	}

	void move(LONG x, LONG y) {
		MoveWindow(hwnd, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 1);
		GetWindowRect(hwnd, &rect);
	}
};