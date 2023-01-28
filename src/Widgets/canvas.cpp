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
	HBRUSH brush = CreateSolidBrush(paint::Color::TRANSPARENTC);
	FillRect(backbufferDC, &rect, brush);
	window->DrawComponents(backbufferDC);
	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
	DeleteObject(brush);
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
	HWND parent):
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent
	), brush(brush) {
	AddComponent(std::make_shared<DigitalClock>(RECT(1000, 500, 1400, 540), paint::Pen(PS_SOLID, 1, paint::Color::GREEN), paint::Brush(paint::Color::TRANSPARENTC)));
}

bool CanvasWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	switch (message) {
	case WM_MOUSEMOVE:
		if (GetInput().mouse.isButtonDown(MouseButton::LEFT)) {
			POINT p(GetInput().mouse.GetMousePosition());
			if (auto c = GetComponentAtPoint(p)) {
				c->move(GetInput().mouse.GetDeltaMousePosition());
			}
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