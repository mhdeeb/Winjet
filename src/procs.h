#pragma once

#include "window.h"

LRESULT CALLBACK HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Log(message, wParam);
	//if (isMouseEvent(message))
	//	return HandleMouse(mouse, hwnd, message, wParam, lParam);
	//if (isKeyboardEvent(message))
	//	return HandleKeyboard(keyboard, hwnd, message, wParam, lParam);
	switch (message) {
	case WM_60_FRAMES:
	case WM_FRAME:
		HandlePaint(hwnd);
		InvalidateRect(hwnd, nullptr, false);
		return 0;
	case WM_PAINT:
		HandlePaint(hwnd);
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_DESTROY:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

namespace procs {
	void init(HINSTANCE hInstance) {
		RegisterWindowClass(hInstance, L"Base", HandleInput, CS_HREDRAW | CS_VREDRAW);
		RegisterWindowClass(hInstance, L"Canvas", HandleInput, CS_HREDRAW | CS_VREDRAW);
		RegisterWindowClass(hInstance, L"WidgetWindow", HandleInput, CS_HREDRAW | CS_VREDRAW);
	}
}