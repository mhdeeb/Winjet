#include "canvas.h"
#include "../messages.h"
#include "../Components/DigitalClock.h"
#include "../includes/json.hpp"
#include <iostream>

void HandlePaint(const WindowClass* window) {
	HWND hwnd = window->GetHwnd();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int width = rect.right;
	int height = rect.bottom;
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);
	HDC backbufferDC = CreateCompatibleDC(hDC);
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(backbufferDC, backbuffer);
	SetBkMode(backbufferDC, TRANSPARENT);
	FillRect(backbufferDC, &rect, window->GetBrush().GetBrush());
	window->DrawComponents(backbufferDC);
	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);
	DeleteDC(hDC);
}

CanvasWindow::CanvasWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	const paint::Brush& brush,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent) :
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		brush,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent
	), brush(brush) {}

bool CanvasWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_LBUTTONDOWN:
		POINT p(GetInput().mouse.GetMousePosition());
		SelectComponentAtPoint(p);
		return true;
	case WM_LBUTTONUP:
		ReleaseSelectedComponent();
		return true;
	case WM_MOUSEMOVE:
		if (GetInput().mouse.isButtonDown(MouseButton::LEFT)) {
			if (auto c = GetSelectedComponent()) {
				c->rmove(GetInput().mouse.GetDeltaMousePosition());
			}
		}
		return true;
	case WM_PAINT:
		HandlePaint(this);
		return true;
	case WM_ERASEBKGND:
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return true;
	default:
		return false;
	}
}

//FIX
nlohmann::json CanvasWindow::Serialize() const
{
	nlohmann::json j;
	j["Component"] = "Window";
	j["ClassName"] = "CanvasWindow";
	int length = GetWindowTextLength(GetHwnd());
	auto name = new wchar_t[length];
	GetWindowTextW(GetHwnd(), name, length);
	j["Name"] = name;
	delete[] name;
	RECT rect;
	GetWindowRect(GetHwnd(), &rect);
	j["Rect"] = { rect.left, rect.top, rect.right, rect.bottom };
	j["Brush"] = brush.Serialize();
	j["Style"] = GetWindowLong(GetHwnd(), GWL_STYLE);
	j["ExStyle"] = GetWindowLong(GetHwnd(), GWL_EXSTYLE);
	return j;
}

//FIX
std::shared_ptr<CanvasWindow> CanvasWindow::Deserialize(const nlohmann::json& data, HINSTANCE hInstance)
{
	auto rect = data["Rect"];
	auto brush = paint::Brush::Deserialize(data["Brush"]);
	auto name = data["Name"].get<std::string>();
	auto style = std::stoul(data["Style"].get<std::string>(), nullptr, 16);
	auto exStyle = std::stoul(data["ExStyle"].get<std::string>(), nullptr, 16);
	auto window = std::make_shared<CanvasWindow>(hInstance,
		rect[0], rect[1], rect[2], rect[3],
		brush,
		std::wstring(name.begin(), name.end()).c_str(),
		CS_HREDRAW | CS_VREDRAW,
		style,
		exStyle,
		nullptr);
	return window;
}