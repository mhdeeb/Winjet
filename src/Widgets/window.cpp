#include "window.h"
#include "canvas.h"
#include "widget.h"
#include "../paint.h"

#include <sstream>

LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	switch (message) {
	case WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

WindowClass::WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName, UINT classStyle, UINT styles, UINT ExStyles, HWND parent, std::string* time_string): hInstance(hInstance), className(className), time_string(time_string) {
	WNDCLASS Class;
	if (!GetClassInfo(hInstance, className, &Class)) {
		WNDCLASS wndclass{};

		wndclass.style = classStyle;
		wndclass.lpfnWndProc = proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(nullptr, IDC_HAND);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndclass.lpszMenuName = nullptr;
		wndclass.lpszClassName = className;

		RegisterClass(&wndclass);
	}

	hwnd = CreateWindowEx(
		ExStyles,
		className,
		windowName,
		styles,
		x,
		y,
		width,
		height,
		parent, nullptr, hInstance, nullptr
	);

	move(0, 0, HWND_BOTTOM);

	if (ExStyles & WS_EX_LAYERED) {
		SetLayeredWindowAttributes(hwnd, TRANSPARENT, NULL, LWA_COLORKEY);
	}

	tracker.cbSize = sizeof(tracker);
	tracker.hwndTrack = hwnd;
	tracker.dwFlags = TME_LEAVE;
	tracker.dwHoverTime = HOVER_DEFAULT;

	SetFont(hFont, 40, BOLD, MODERN, L"Curier New");
	hBrush = CreateSolidBrush(TRANSPARENT);
}

WindowClass::~WindowClass() {
	DeleteObject(hBrush);
	DeleteObject(hFont);
}

HWND WindowClass::GetHwnd() const {
	return hwnd;
}

HFONT WindowClass::GetFont() const {
	return hFont;
}

HBRUSH WindowClass::GetBrush() const {
	return hBrush;
}

std::string WindowClass::GetTimeString() const {
	if (time_string)
		return *time_string;
	return "";
}

Input WindowClass::GetInput() const {
	return input;
}

void WindowClass::SetTimeString(std::string* time_string) {
	this->time_string = time_string;
}

std::wstring WindowClass::Serialize() const {
	WCHAR windowName[256];
	GetWindowText(hwnd, windowName, 256);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	WCHAR className[256];
	GetClassName(hwnd, className, 256);
	UINT styles = GetWindowLong(hwnd, GWL_STYLE);
	UINT ExStyles = GetWindowLong(hwnd, GWL_EXSTYLE);

	std::wstringstream ss;
	ss << className << ' ' << rect.left << ' ' << rect.top << ' ' << rect.right << ' ' << rect.bottom << ' ' << windowName << ' ' << std::hex << styles << ' ' << std::hex << ExStyles << std::endl;
	return ss.str();
}

std::shared_ptr<WindowClass> WindowClass::DeSerialize(const std::wstring& line, const HINSTANCE& HInstance) {
	std::wstringstream ss(line);
	std::wstring className;
	ss >> className;
	RECT rect;
	ss >> rect.left >> rect.top >> rect.right >> rect.bottom;
	std::wstring windowName;
	ss >> windowName;
	UINT styles;
	ss >> std::hex >> styles;
	UINT ExStyles;
	ss >> std::hex >> ExStyles;
	if (className == L"CanvasWindow") {
		return std::make_shared<CanvasWindow>(HInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, windowName.c_str(), NULL, styles, ExStyles, HWND_DESKTOP);
	} else if (className == L"WidgetWindow") {
		return std::make_shared<WidgetWindow>(HInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, windowName.c_str(), NULL, styles, ExStyles, HWND_DESKTOP);
	} else
		throw std::logic_error("Invalid Class Name");
}

void WindowClass::move(LONG x, LONG y, HWND insertAfter) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	//POINT p{rect.left + x, rect.top + y};
	//SIZE s{rect.right - rect.left, rect.bottom - rect.top};
	//if (GetWindowLong(hwnd, GWL_EXSTYLE) == WS_EX_LAYERED && false) {
	//	UpdateLayeredWindow(hwnd, nullptr, &p, &s, nullptr, nullptr, TRANSPARENT, nullptr, ULW_COLORKEY);
	//	SetWindowPos(hwnd, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//} else {
	SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE);
	//}
}

void WindowClass::HandleInput(UINT message, WPARAM wParam) {
	switch (message) {
		using enum MouseButton;
	case WM_MOUSELEAVE:
		input.mouse.ButtonState.fill(false);
		input.mouse.inWindow = false;
		return;
	case WM_MOUSEMOVE:
		POINT p;
		GetCursorPos(&p);
		input.mouse.SetMousePosition(p);
		if (!input.mouse.inWindow) {
			TrackMouseEvent(&tracker);
			input.mouse.inWindow = true;
		}
		return;
	case WM_LBUTTONDOWN:
		input.mouse.SetButtonState(LEFT, true);
		return;
	case WM_LBUTTONUP:
		input.mouse.SetButtonState(LEFT, false);
		return;
	case WM_RBUTTONDOWN:
		input.mouse.SetButtonState(RIGHT, true);
		return;
	case WM_RBUTTONUP:
		input.mouse.SetButtonState(RIGHT, false);
		return;
	case WM_MBUTTONDOWN:
		input.mouse.SetButtonState(MIDDLE, true);
		return;
	case WM_MBUTTONUP:
		input.mouse.SetButtonState(MIDDLE, false);
		return;
	case WM_KEYDOWN:
		input.keyboard.SetKeyState(int(wParam), true);
		return;
	case WM_KEYUP:
		input.keyboard.SetKeyState(int(wParam), false);
		return;
	default:
		return;
	}
}