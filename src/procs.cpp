#include "procs.h"

#include <map>
#include <string>

#include "messages.h"
#include "windowinput.h"

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

void procs::init(HINSTANCE hInstance) {
	RegisterWindowClass(hInstance, L"HiddenWindow", DefWindowProc);
	RegisterWindowClass(hInstance, L"CanvasWindow", HandleInput);
	RegisterWindowClass(hInstance, L"WidgetWindow", HandleInput);
}