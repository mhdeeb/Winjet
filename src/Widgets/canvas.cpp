#include "canvas.h"
#include "../messages.h"

void HandlePaint(WindowClass* window);

CanvasWindow::CanvasWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent,
	std::string* time_string):
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent,
		time_string
	) {}

bool CanvasWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	switch (message) {
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