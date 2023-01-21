#include "windowinput.h"

#include "messages.h"
#include "my_ssql.h"
#include "window.h"

LRESULT HandleMouse(mouseInput& mouse, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_MOUSELEAVE:
		mouse.isLeftDown = false;
		mouse.isRightDown = false;
		mouse.isMiddleDown = false;
		mouse.inWindow = false;
		return 0;
	case WM_MOUSEMOVE:
		if (!mouse.inWindow) {
			TrackMouseEvent(&mouse.tracker);
			mouse.inWindow = true;
		}
		POINT p = mouse.p;
		GetCursorPos(&mouse.p);
		mouse.delta.x = mouse.p.x - p.x;
		mouse.delta.y = mouse.p.y - p.y;
		if (mouse.isLeftDown) {
			move(hwnd, mouse.delta.x, mouse.delta.y);
		}
		return 0;
	case WM_LBUTTONDOWN:
		mouse.isLeftDown = true;
		return 0;
	case WM_LBUTTONUP:
		mouse.isLeftDown = false;
		return 0;
	case WM_RBUTTONDOWN:
		mouse.isRightDown = true;
		return 0;
	case WM_RBUTTONUP:
		mouse.isRightDown = false;
		return 0;
	case WM_MBUTTONDOWN:
		mouse.isMiddleDown = true;
		return 0;
	case WM_MBUTTONUP:
		mouse.isMiddleDown = false;
		return 0;
	case WM_MOUSEWHEEL:
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT HandleKeyboard(keyboardInput& keyboard, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_KEYDOWN:
		keyboard.isKeyDown = true;
		return 0;
	case WM_KEYUP:
		keyboard.isKeyDown = false;
		return 0;
	case WM_CHAR:
		keyboard.charCode = wParam;
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

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
	//RECT text(rect);
	//text.left += 1500;
	//text.top += 20;
	DrawText(backbufferDC, szBuffer, int(wcommand.size()), &rect, BS_CENTER);	// Draw Text

	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	RestoreDC(backbufferDC, savedDC);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);

	EndPaint(hwnd, &ps);
	//SwapBuffers(hDC);
	//ReleaseDC(hwnd, hDC);
	DeleteObject(SelectObject(hDC, hTmpBr));
	DeleteObject(SelectObject(hDC, hTmpFnt));
}

input::input(HWND hwnd, HINSTANCE hInstance): hInstance(hInstance), mouse(hwnd), hwnd(hwnd) {}

input::~input() {
	timer.stop();
	timer2.stop();
}

LRESULT input::HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	if (isMouseEvent(message))
		return HandleMouse(mouse, hwnd, message, wParam, lParam);
	if (isKeyboardEvent(message))
		return HandleKeyboard(keyboard, hwnd, message, wParam, lParam);
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