#pragma once

#include "window.h"

class CanvasWindow: public WindowClass {
public:
	CanvasWindow(HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CHILD, UINT ExStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT, HWND parent = HWND_DESKTOP, std::string* time_string = nullptr);
	bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) override;
};