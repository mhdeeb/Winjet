#include "widget.h"
#include "../messages.h"

void HandlePaint(WindowClass* window) {
	RECT rect;
	GetClientRect(window->GetHwnd(), &rect);
	int width = rect.right;
	int height = rect.bottom;
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(window->GetHwnd(), &ps);
	HDC backbufferDC = CreateCompatibleDC(hDC);
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(backbufferDC, backbuffer);

	SetBkMode(backbufferDC, TRANSPARENT);

	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);
	DeleteDC(hDC);
	EndPaint(window->GetHwnd(), &ps);
}

WidgetWindow::WidgetWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent):
	WindowClass(hInstance,
		L"WidgetWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent
	) {}

bool WidgetWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	if (GetInput().keyboard.isKeyPressed(VK_ESCAPE))
		PostMessage(nullptr, WM_QUIT, 0, 0);
	switch (message) {
	case WM_MOUSEMOVE:
		if (GetInput().mouse.isButtonDown(MouseButton::LEFT)) {
			POINT p(GetInput().mouse.GetDeltaMousePosition());
			move(p.x, p.y);
		}
	case WM_TIMER:
		InvalidateRect(GetHwnd(), nullptr, false);
		HandlePaint(this);
		return true;
	case WM_PAINT:
		HandlePaint(this);
		return true;
	case WM_ERASEBKGND:
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return true;
	default:
		return false;
	}
}