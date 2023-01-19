#pragma once

#include "test.h"
#include "my_ssql.h"
#include "test.h"
#include "window.h"

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

struct input {
	window win;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	mouseInput mouse;
	keyboardInput keyboard;

	input(HWND hwnd, HINSTANCE hInstance): win(hwnd, hInstance), mouse(hwnd) {}

	LRESULT HandleMouse() {
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
				win.move(mouse.delta.x, mouse.delta.y);
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
			return DefWindowProc(win.hwnd, message, wParam, lParam);
		}
	}

	LRESULT HandleKeyboard() {
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
			return DefWindowProc(win.hwnd, message, wParam, lParam);
		}
	}

	LRESULT HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		this->win.hwnd = hwnd;
		this->message = message;
		this->wParam = wParam;
		this->lParam = lParam;
		Log();
		if (isMouseEvent(message))
			return HandleMouse();
		if (isKeyboardEvent(message))
			return HandleKeyboard();
		switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	void Log() const {
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
};
