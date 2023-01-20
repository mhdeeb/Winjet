#pragma once

#include "my_ssql.h"
#include "window.h"
#include "test.h"
#include "CallBackTimer.h"

#include <commctrl.h>
#include <format>
#include <chrono>

using namespace test;

#define WM_FRAME (WM_USER + 1)
#define WM_60_FRAMES (WM_USER + 2)

std::string exec(const char*);

std::string time_string;
HFONT hFont;
HBRUSH hBrush;

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

// Generic Font family's
enum fontfamily {
	BY_NAME,	// use font name instead of abstract type
	MODERN,		// monospace with or without serifs
	ROMAN,		// variable width with serifs
	SCRIPT,		// like handwriting
	SWISS		  // variable width without serifs
};

const unsigned char PLAIN = 0x00;
const unsigned char BOLD = 0x01;
const unsigned char ITALICIZED = 0x02;
const unsigned char UNDERLINED = 0x04;
const unsigned char STRIKEOUT = 0x08;

void SetFont(HFONT& fontObject, const int iSize, const unsigned short usStyle, const fontfamily ffFamily, const wchar_t* cFontName) {
	LOGFONT lf{sizeof(lf)};

	lf.lfHeight = iSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;

	if ((usStyle & BOLD) == BOLD) {
		lf.lfWeight = FW_BOLD;
	} else {
		lf.lfWeight = FW_NORMAL;
	}

	if ((usStyle & ITALICIZED) == ITALICIZED) {
		lf.lfItalic = true;
	} else {
		lf.lfItalic = false;
	}

	if ((usStyle & UNDERLINED) == UNDERLINED) {
		lf.lfUnderline = true;
	} else {
		lf.lfUnderline = false;
	}

	if ((usStyle & STRIKEOUT) == STRIKEOUT) {
		lf.lfStrikeOut = true;
	} else {
		lf.lfStrikeOut = false;
	}

	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;

	switch (ffFamily) {
	case MODERN:
		lf.lfPitchAndFamily = FF_MODERN;
		break;

	case ROMAN:
		lf.lfPitchAndFamily = FF_ROMAN;
		break;

	case SCRIPT:
		lf.lfPitchAndFamily = FF_SCRIPT;
		break;

	case SWISS:
		lf.lfPitchAndFamily = FF_SWISS;
		break;

	default:
		lf.lfPitchAndFamily = FF_DONTCARE;
		break;
	}

	lf.lfPitchAndFamily |= DEFAULT_PITCH;
	if (cFontName)
		wcscpy_s(lf.lfFaceName, cFontName);

	fontObject = CreateFontIndirect(&lf);
}

void setFont() {
	SetFont(hFont, 40, BOLD, MODERN, L"Curier New");
}

void setBrush() {
	hBrush = CreateSolidBrush(RGB(0, 0, 1));
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

std::string return_current_time_and_date() {
	auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	return std::format("{:%Y-%m-%d %X}", now);
}

struct input {
	HINSTANCE hInstance;
	mouseInput mouse;
	keyboardInput keyboard;
	CallBackTimer timer;
	CallBackTimer timer2;
	HWND hwnd;
	input(HWND hwnd, HINSTANCE hInstance): hInstance(hInstance), mouse(hwnd), hwnd(hwnd) {
		move(hwnd, 0, 0);
		setFont();
		setBrush();
		timer.start(1000, [hwnd](std::string& output) {
			output = return_current_time_and_date();
		SendMessage(hwnd, WM_FRAME, 0, 0);
			}, std::ref(time_string));
		//timer2.start(16, [hwnd]() {SendMessage(hwnd, WM_60_FRAMES, 0, 0); });
	}
	~input() {
		timer.stop();
		timer2.stop();
	}

	LRESULT HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
};
