#include "window.h"
#include "messages.h"
#include "paint.h"

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
	ss << className << ' ' << rect.left << ' ' << rect.top << ' ' << rect.right << ' ' << rect.bottom << ' ' << windowName << ' ' << styles << ' ' << ExStyles << std::endl;
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
	ss >> styles;
	UINT ExStyles;
	ss >> ExStyles;
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
	SetWindowPos(hwnd, insertAfter, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 0);
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

WidgetWindow::WidgetWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent,
	std::string* time_string):
	WindowClass(hInstance,
		L"WidgetWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent,
		time_string
	) {}

void HandlePaint(WindowClass* window) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(window->GetHwnd(), &ps);

	HDC backbufferDC = CreateCompatibleDC(hDC);
	RECT rect;
	GetClientRect(window->GetHwnd(), &rect);
	int width = rect.right;
	int height = rect.bottom;
	HBITMAP backbuffer = CreateCompatibleBitmap(hDC, width, height);
	int savedDC = SaveDC(backbufferDC);
	SelectObject(backbufferDC, backbuffer);

	HBRUSH hTmpBr = (HBRUSH)SelectObject(backbufferDC, window->GetBrush());
	HFONT hTmpFnt = (HFONT)SelectObject(backbufferDC, window->GetFont());
	SetBkMode(backbufferDC, TRANSPARENT);
	SetTextColor(backbufferDC, RGB(128, 128, 255));

	FillRect(backbufferDC, &rect, window->GetBrush());

	std::string time_string = window->GetTimeString();

	std::wstring wcommand(time_string.begin(), time_string.end());
	const TCHAR* szBuffer = wcommand.c_str();
	RECT text(rect);
	text.left += 1500;
	text.top += 20;
	DrawText(backbufferDC, szBuffer, int(wcommand.size()), &rect, BS_CENTER);

	BitBlt(hDC, 0, 0, width, height, backbufferDC, 0, 0, SRCCOPY);
	RestoreDC(backbufferDC, savedDC);
	DeleteObject(backbuffer);
	DeleteDC(backbufferDC);

	EndPaint(window->GetHwnd(), &ps);
	//SwapBuffers(hDC);
	ReleaseDC(window->GetHwnd(), hDC);
	DeleteObject(SelectObject(hDC, hTmpBr));
	DeleteObject(SelectObject(hDC, hTmpFnt));
}

bool WidgetWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	//Log(message, wParam);
	if (GetInput().keyboard.isKeyPressed(VK_ESCAPE))
		PostMessage(nullptr, WM_QUIT, 0, 0);
	switch (message) {
	case WM_MOUSEMOVE:
		if (GetInput().mouse.isButtonDown(MouseButton::LEFT)) {
			POINT p(GetInput().mouse.GetDeltaMousePosition());
			move(p.x, p.y);
		}
	case WM_60_FRAMES:
		HandlePaint(this);
		InvalidateRect(GetHwnd(), nullptr, false);
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

CanvasWindow::CanvasWindow(HINSTANCE hInstance,
	int x, int y, int width, int height,
	LPCWSTR windowName,
	UINT classStyle,
	UINT styles,
	UINT ExStyles,
	HWND parent,
	std::string* time_string):
	WindowClass(hInstance,
		L"CanvasWindow",
		x, y, width, height,
		windowName,
		classStyle,
		styles,
		ExStyles,
		parent,
		time_string
	) {}

bool CanvasWindow::WinProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_60_FRAMES:
		HandlePaint(this);
		InvalidateRect(GetHwnd(), nullptr, false);
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