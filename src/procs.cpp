#include "procs.h"

#include <map>
#include <string>

#include "messages.h"
#include "windowinput.h"

void HandlePaint(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);

	HDC backbufferDC = CreateCompatibleDC(hDC);
	RECT rect;
	GetClientRect(hwnd, &rect);
	int width = rect.right;
	int height = rect.bottom;
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	int savedDC = SaveDC(backbufferDC);
	SelectObject(backbufferDC, backbuffer);

	HBRUSH hTmpBr = (HBRUSH)SelectObject(backbufferDC, hBrush);
	HFONT hTmpFnt = (HFONT)SelectObject(backbufferDC, hFont);
	SetBkMode(backbufferDC, TRANSPARENT);
	SetTextColor(backbufferDC, RGB(128, 128, 255));

	FillRect(backbufferDC, &rect, hBrush);														// Clear Window

	std::wstring wcommand(time_string.begin(), time_string.end());
	const TCHAR* szBuffer = wcommand.c_str();
	RECT text(rect);
	text.left += 1500;
	text.top += 20;
	DrawText(backbufferDC, szBuffer, int(wcommand.size()), &rect, BS_CENTER);	// Draw Text

	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	RestoreDC(backbufferDC, savedDC);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);

	EndPaint(hwnd, &ps);
	SwapBuffers(hDC);
	ReleaseDC(hwnd, hDC);
	DeleteObject(SelectObject(hDC, hTmpBr));
	DeleteObject(SelectObject(hDC, hTmpFnt));
}

LRESULT CALLBACK HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Log(message, wParam);
	switch (message) {
	case WM_60_FRAMES:
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

void procs::init(HINSTANCE hInstance) {
	RegisterWindowClass(hInstance, L"HiddenWindow", DefWindowProc);
	RegisterWindowClass(hInstance, L"CanvasWindow", HandleInput);
	RegisterWindowClass(hInstance, L"WidgetWindow", HandleInput);
}