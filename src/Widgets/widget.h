#pragma once

#include "window.h"

class WidgetWindow: public WindowClass {
public:
	WidgetWindow(HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP | WS_VISIBLE | WS_CHILD, UINT ExStyles = WS_EX_LAYERED, HWND parent = HWND_DESKTOP);
	bool WinProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
};