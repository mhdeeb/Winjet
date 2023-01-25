#include "widget.h"
#include "../messages.h"

void HandlePaint(WindowClass* window) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(window->GetHwnd(), &ps);

	HDC backbufferDC = CreateCompatibleDC(hDC);
	RECT rect;
	GetClientRect(window->GetHwnd(), &rect);
	int width = rect.right;
	int height = rect.bottom;
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	int savedDC = SaveDC(backbufferDC);
	SelectObject(backbufferDC, backbuffer);

	HBRUSH hTmpBr = (HBRUSH)SelectObject(backbufferDC, window->GetBrush());
	HFONT hTmpFnt = (HFONT)SelectObject(backbufferDC, window->GetFont());
	SetBkMode(backbufferDC, TRANSPARENT);
	SetTextColor(backbufferDC, RGB(128, 128, 255));

	FillRect(backbufferDC, &rect, window->GetBrush());

	std::string time_string = window->GetTimeString();

	std::wstring wcommand(time_string.begin(), time_string.end());
	const TCHAR* szBuffer = wcommand.c_str();
	RECT text(rect);
	text.left += 1500;
	text.top += 20;
	DrawText(backbufferDC, szBuffer, int(wcommand.size()), &rect, BS_CENTER);

	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	RestoreDC(backbufferDC, savedDC);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);

	EndPaint(window->GetHwnd(), &ps);
	//SwapBuffers(hDC);
	ReleaseDC(window->GetHwnd(), hDC);
	DeleteObject(SelectObject(hDC, hTmpBr));
	DeleteObject(SelectObject(hDC, hTmpFnt));
}

WidgetWindow::WidgetWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent,
	std::string* time_string):
	WindowClass(hInstance,
		L"WidgetWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent,
		time_string
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
	case WM_60_FRAMES:
		HandlePaint(this);
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