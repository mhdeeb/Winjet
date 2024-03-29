#include "messages.h"
#include "my_ssql.h"
#include "printer.h"

#include <WinUser.h>
#include <windef.h>
#include <stdio.h>

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

void Log(UINT message, WPARAM wParam) {
	using namespace printer;
	if (isMouseEvent(message)) {
		switch (message) {
		case WM_LBUTTONDBLCLK:
			print("WM_LBUTTONDBLCLK");
			break;
		case WM_MOUSEMOVE:
			POINT p;
			GetCursorPos(&p);
			printv("WM_MOUSEMOVE: ({}, {})\n", p.x, p.y);
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
			print("Unhandled Mouse Event:", my_ssql::code_to_name_msg(message));
			break;
		}
	} else if (isKeyboardEvent(message)) {
		std::string key = my_ssql::code_to_name_vk(char(wParam));
		switch (message) {
		case WM_KEYDOWN:
			if (!key.empty())
				print("Key Down:", key);
			else
				print("Key Down:", char(wParam));
			break;
		case WM_KEYUP:
			if (!key.empty())
				print("Key Up:", key);
			else
				print("Key Up:", char(wParam));
			break;
		case WM_CHAR:
			printf("Char: %c\n", char(wParam));
			break;
		default:
			print("Unhandled Keyboard Event:", key);
			break;
		}
	} else {
		if (std::string name = my_ssql::code_to_name_msg(message); !name.empty())
			print("Unhandled Event:", name);
		else
			print("Unhandled Event:", message);
	}
}