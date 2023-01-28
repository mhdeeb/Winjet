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
	HWND parent) :
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent
	), brush(brush) {
	AddComponent(std::make_shared<DigitalClock>(RECT(1000, 500, 1400, 540), paint::Pen(PS_SOLID, 1, paint::Color::GREEN), paint::Brush(paint::Color::RED)));
	AddComponent(std::make_shared<DigitalClock>(RECT(500, 500, 900, 540), paint::Pen(PS_SOLID, 1, paint::Color::GREEN), paint::Brush(paint::Color::GREEN)));
	AddComponent(std::make_shared<DigitalClock>(RECT(500, 800, 900, 840), paint::Pen(PS_SOLID, 1, paint::Color::GREEN), paint::Brush(paint::Color::BLUE)));
}

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
				InvalidateRect(GetHwnd(), nullptr, false);
			}
		}
		return true;
	case WM_TIMER:
		InvalidateRect(GetHwnd(), nullptr, false);
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