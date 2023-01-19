#pragma once

#include "my_ssql.h"
#include "window.h"
#include "test.h"

#include <commctrl.h>

using namespace test;

bool isMouseEvent(UINT message) {
	switch (message) {
	case WM_CAPTURECHANGED:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEACTIVATE:
	case WM_MOUSEHOVER:
	case WM_MOUSEHWHEEL:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCMOUSEHOVER:
	case WM_NCMOUSELEAVE:
	case WM_NCMOUSEMOVE:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCXBUTTONDBLCLK:
	case WM_NCXBUTTONDOWN:
	case WM_NCXBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		return true;
	default:
		return false;
	}
}

bool isKeyboardEvent(UINT message) {
	switch (message) {
	case WM_APPCOMMAND:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_HOTKEY:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSDEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_UNICHAR:
		return true;
	default:
		return false;
	}
}

struct mouseInput {
	POINT p = {0, 0};
	POINT delta = {0, 0};
	bool isLeftDown = false;
	bool isRightDown = false;
	bool isMiddleDown = false;
	bool inWindow = false;
	TRACKMOUSEEVENT tracker;
	explicit mouseInput(HWND hwnd) {
		tracker.cbSize = sizeof(tracker);
		tracker.hwndTrack = hwnd;
		tracker.dwFlags = TME_LEAVE;
		tracker.dwHoverTime = HOVER_DEFAULT;
	}
};

struct keyboardInput {
	bool isKeyDown = false;
	WPARAM charCode = 0;
};

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
	RECT rect;
	GetClientRect(hwnd, &rect);
	TCHAR szBuffer[] = TEXT("Hello World!");
	HFONT hFont = CreateFont(80, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, TEXT("SYSTEM_FIXED_FONT"));
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);
	HBRUSH hTmpBr = (HBRUSH)SelectObject(hDC, hBrush);
	HFONT hTmpFnt = (HFONT)SelectObject(hDC, hFont);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(128, 0, 0));
	FillRect(hDC, &rect, hBrush);
	DrawText(hDC, szBuffer, 13, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	EndPaint(hwnd, &ps);
	DeleteObject(SelectObject(hDC, hTmpBr));
	DeleteObject(SelectObject(hDC, hTmpFnt));
}

void Log(UINT message, WPARAM wParam) {
	if (isMouseEvent(message)) {
		switch (message) {
		case WM_MOUSEMOVE:
			POINT p;
			GetCursorPos(&p);
			printf("\rMouse Position: (%d, %d)\n", int(p.x), int(p.y));
			fflush(stdout);
			break;
		case WM_LBUTTONDOWN:
			printf("Mouse Button: LEFT_DOWN\n");
			break;
		case WM_LBUTTONUP:
			printf("Mouse Button: LEFT_UP\n");
			break;
		case WM_RBUTTONDOWN:
			printf("Mouse Button: RIGHT_DOWN\n");
			break;
		case WM_RBUTTONUP:
			printf("Mouse Button: RIGHT_UP\n");
			break;
		case WM_MBUTTONDOWN:
			printf("Mouse Button: MIDDLE_DOWN\n");
			break;
		case WM_MBUTTONUP:
			printf("Mouse Button: MIDDLE_UP\n");
			break;
		case WM_MOUSEWHEEL:
			printf("Mouse Wheel: %d\n", GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		case WM_MOUSELEAVE:
			print("Mouse: LEAVE");
			break;
		default:
			printv("Unhandled Mouse Event: {}\n", my_ssql::code_to_name_msg(message));
		}
	} else if (isKeyboardEvent(message)) {
		std::string key = my_ssql::code_to_name_vk(char(wParam));
		switch (message) {
		case WM_KEYDOWN:
			if (!key.empty())
				printv("Key Down: {}\n", key);
			else
				printv("Key Down: {}\n", char(wParam));
			break;
		case WM_KEYUP:
			if (!key.empty())
				printv("Key Up: {}\n", key);
			else
				printv("Key Up: {}\n", char(wParam));
			break;
		case WM_CHAR:
			printf("Char: %c\n", char(wParam));
			break;
		default:
			printv("Unhandled Keyboard Event: {}\n", key);
		}
	} else
		printv("Other Event: {}\n", my_ssql::code_to_name_msg(message));
}

struct input {
	HINSTANCE hInstance;
	mouseInput mouse;
	keyboardInput keyboard;
	input(HWND hwnd, HINSTANCE hInstance): hInstance(hInstance), mouse(hwnd) {
		CheckDarkMode(hwnd);
		move(hwnd, 100, 100);
	}

	LRESULT HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		//Log(message, wParam);
		if (isMouseEvent(message))
			return HandleMouse(mouse, hwnd, message, wParam, lParam);
		if (isKeyboardEvent(message))
			return HandleKeyboard(keyboard, hwnd, message, wParam, lParam);
		switch (message) {
		case WM_PAINT:
		case WM_ERASEBKGND:
			HandlePaint(hwnd);
			return 0;
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
};
