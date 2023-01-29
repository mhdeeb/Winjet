#include "canvas.h"
#include "../messages.h"
#include "../Components/DigitalClock.h"

void HandlePaint(const WindowClass* window) {
	HWND hwnd = window->GetHwnd();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int width = rect.right;
	int height = rect.bottom;
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);
	HDC backbufferDC = CreateCompatibleDC(hDC);
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(backbufferDC, backbuffer);
	SetBkMode(backbufferDC, TRANSPARENT);
	FillRect(backbufferDC, &rect, window->GetBrush().GetBrush());
	window->DrawComponents(backbufferDC);
	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);
	DeleteDC(hDC);
}

CanvasWindow::CanvasWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	const paint::Brush& brush,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent) :
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		brush,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent
	), brush(brush) {}

bool CanvasWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_LBUTTONDOWN:
		POINT p(GetInput().mouse.GetMousePosition());
		SelectComponentAtPoint(p);
		return true;
	case WM_LBUTTONUP:
		ReleaseSelectedComponent();
		return true;
	case WM_MOUSEMOVE:
		if (GetInput().mouse.isButtonDown(MouseButton::LEFT)) {
			if (auto c = GetSelectedComponent()) {
				c->rmove(GetInput().mouse.GetDeltaMousePosition());
			}
		}
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