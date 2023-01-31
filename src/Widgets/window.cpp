#include "window.h"
#include "canvas.h"
#include "../paint.h"
#include "../messages.h"
#include "../Components/DigitalClock.h"

#include <sstream>
#include <ranges>
#include <algorithm>

LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	switch (message) {
	case WM_LBUTTONDOWN:
		SetCapture(hwnd);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		break;
	case WM_ERASEBKGND:
		return true;
	case WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

WindowClass::WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, const paint::Brush& brush, LPCWSTR windowName, UINT classStyle, UINT styles, UINT ExStyles, HWND parent) : hInstance(hInstance), className(className), brush(brush) {
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

void WindowClass::move(LONG x, LONG y, HWND insertAfter) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	POINT p{ rect.left + x, rect.top + y };
	SIZE s{ rect.right - rect.left, rect.bottom - rect.top };
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
	for (auto& component : components) {
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

std::vector<std::shared_ptr<Component>>* WindowClass::GetComponents()
{
	return &components;
}

void WindowClass::SelectComponentAtPoint(const POINT& point)
{
	SelectedComponent = GetComponentAtPoint(point);
}

void WindowClass::ReleaseSelectedComponent()
{
	SelectedComponent = nullptr;
}

std::shared_ptr<Component> WindowClass::GetSelectedComponent() const
{
	return SelectedComponent;
}

std::shared_ptr<Component> WindowClass::GetComponentAtPoint(const POINT& point) const {
	for (auto& component : std::ranges::reverse_view(components)) {
		if (component->IsPointInComponent(point)) {
			return component;
		}
	}
	return nullptr;
}

void WindowClass::SetBrush(const paint::Brush& brush) {
	this->brush = brush;
}

paint::Brush WindowClass::GetBrush() const {
	return brush;
}