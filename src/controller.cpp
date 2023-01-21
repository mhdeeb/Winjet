#include "controller.h"
#include "windowinput.h"
#include "messages.h"
#include "paint.h"
#include "util.h"
#include "procs.h"

void EachSixtyFrames(const Controller* cont) {
	for (auto& [Class, windows] : cont->classes) {
		for (auto window : windows) {
			PostMessage(window, WM_60_FRAMES, 0, 0);
		}
	}
}

void UpdateTime(Controller* cont) {
	cont->time_string = return_current_time_and_date();
}

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	SetFont(hFont, 40, BOLD, MODERN, L"Curier New");
	hBrush = CreateSolidBrush(TRANSPARENT);

	AddTask(1000, UpdateTime, this);
	AddTask(16, EachSixtyFrames, this);

	procs::init(hInstance);
}

Controller::~Controller() {
	timers.stopAll();

	for (auto [Class, windows] : classes) {
		for (auto window : windows) {
			DestroyWindow(window);
		}
		windows.clear();
		UnregisterClass(Class, hInstance);
	}
	classes.clear();

	DeleteObject(hBrush);
	DeleteObject(hFont);
}

int Controller::run() {
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);
}

HWND Controller::AddWindow(LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName, UINT styles, UINT ExStyles, HWND parent) {
	HWND window = CreateWidgetWindow(
		className,
		hInstance,
		x, y, width, height,
		windowName,
		styles,
		ExStyles,
		parent);
	if (ExStyles & WS_EX_LAYERED) {
		SetLayeredWindowAttributes(window, TRANSPARENT, NULL, LWA_COLORKEY);
	}
	classes[className].push_back(window);
	return window;
}

void Controller::LoadWindows(const char* filepath) {
	HWND base = AddWindow(
		L"HiddenWindow",
		0, 0, 0, 0,
		L"Base",
		WS_POPUP,
		WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
		HWND_DESKTOP
	);
	HWND canvas = AddWindow(
		L"CanvasWindow",
		0, 0, 0, 0,
		L"Canvas",
		WS_POPUP | WS_MAXIMIZE | WS_VISIBLE,
		WS_EX_LAYERED | WS_EX_TRANSPARENT,
		base
	);
	AddWindow(
		L"WidgetWindow",
		0, 0, 100, 100,
		L"test",
		WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CHILD,
		WS_EX_LAYERED | WS_EX_TRANSPARENT,
		canvas
	);
}

void Controller::SaveWindows(const char* filepath) const {}