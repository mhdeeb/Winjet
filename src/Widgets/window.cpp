#include "window.h"
#include "canvas.h"
#include "widget.h"
#include "../paint.h"
#include "../messages.h"
#include "../Components/DigitalClock.h"

#include <sstream>
#include <ranges>
#include <algorithm>

LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	switch (message) {
	case WM_CREATE:
		SetTimer(hwnd, NULL, 16, nullptr);
	case WM_ERASEBKGND:
		return true;
	case WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

WindowClass::WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName, UINT classStyle, UINT styles, UINT ExStyles, HWND parent): hInstance(hInstance), className(className) {
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
		wndclass.hbrBackground = nullptr;
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
		SetLayeredWindowAttributes(hwnd, paint::Color::TRANSPARENTC, 255, LWA_COLORKEY | LWA_ALPHA);
	}

	tracker.cbSize = sizeof(tracker);
	tracker.hwndTrack = hwnd;
	tracker.dwFlags = TME_LEAVE;
	tracker.dwHoverTime = HOVER_DEFAULT;
}

WindowClass::~WindowClass() {
	components.clear();
	DestroyWindow(hwnd);
}

HWND WindowClass::GetHwnd() const {
	return hwnd;
}

Input WindowClass::GetInput() const {
	return input;
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
	ss << className << ' ' << rect.left << ' ' << rect.top << ' ' << rect.right << ' ' << rect.bottom << ' ' << windowName << ' ' << std::hex << styles << ' ' << std::hex << ExStyles;
	return ss.str();
}

std::shared_ptr<WindowClass> WindowClass::DeSerialize(const std::wstring& line, const HINSTANCE& HInstance, int& parentId) {
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
	ss >> parentId;

	if (className == L"CanvasWindow") {
		return std::make_shared<CanvasWindow>(HInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, paint::Brush(paint::Color::TRANSPARENTC), windowName.c_str(), NULL, styles, ExStyles, HWND_DESKTOP);
	} else if (className == L"WidgetWindow") {
		return std::make_shared<WidgetWindow>(HInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, windowName.c_str(), NULL, styles, ExStyles, HWND_DESKTOP);
	} else
		throw std::logic_error("Invalid Class Name");
}

void WindowClass::move(LONG x, LONG y, HWND insertAfter) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	POINT p{rect.left + x, rect.top + y};
	SIZE s{rect.right - rect.left, rect.bottom - rect.top};
	if (GetWindowLong(hwnd, GWL_EXSTYLE) == WS_EX_LAYERED && false) {
		UpdateLayeredWindow(hwnd, nullptr, &p, &s, nullptr, nullptr, TRANSPARENT, nullptr, ULW_COLORKEY);
		SetWindowPos(hwnd, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	} else {
		SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE | SWP_NOZORDER);
	}
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

void WindowClass::DrawComponents(const HDC& hdc) const {
	for (auto component : components) {
		component->paint(hdc);
	}
}

void WindowClass::AddComponent(std::shared_ptr<Component> component) {
	components.push_back(component);
}

void WindowClass::RemoveComponent(std::shared_ptr<Component> component) {
	const auto ret = std::ranges::remove(components, component);
	components.erase(ret.begin(), ret.end());
}

std::shared_ptr<Component> WindowClass::GetComponentAtPoint(const POINT& point) const {
	for (auto component : std::ranges::reverse_view(components)) {
		if (component->IsPointInComponent(point)) {
			return component;
		}
	}
	return nullptr;
}