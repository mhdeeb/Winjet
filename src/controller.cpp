#include "controller.h"
#include "windowinput.h"
#include "messages.h"
#include "paint.h"
#include "window.h"
#include "util.h"

LRESULT CALLBACK HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Log(message, wParam);
	//if (isMouseEvent(message))
	//	return HandleMouse(mouse, hwnd, message, wParam, lParam);
	//if (isKeyboardEvent(message))
	//	return HandleKeyboard(keyboard, hwnd, message, wParam, lParam);
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

void Controller::UpdateTime() {}

void EachSixtyFrames(HWND hwnd) {
	PostMessage(hwnd, WM_60_FRAMES, 0, 0);
}

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	RegisterWindowClass(hInstance, HIDDENCLASS, HandleInput);
	RegisterWindowClass(hInstance, CANVASCLASS, HandleInput);

	base = CreateHiddenWindow(hInstance);
	canvas = CreateCanvasOverlayWindow(hInstance, base);

	SetFont(hFont, 40, BOLD, MODERN, L"Curier New");
	hBrush = CreateSolidBrush(TRANSPARENT);

	AddTask(1000, [this]() {
		time_string = return_current_time_and_date();
		});
}

Controller::~Controller() {
	timers.stopAll();

	for (auto& window : windows) {
		DestroyWindow(window);
	}
	DestroyWindow(canvas);
	DestroyWindow(base);

	for (auto const& [name, Class] : classes) {
		UnregisterClass(name, hInstance);
	}
	UnregisterClass(CANVASCLASS, hInstance);
	UnregisterClass(HIDDENCLASS, hInstance);
}

int Controller::run() {
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);
}

void Controller::LoadWindows(const char* filepath) {
	AddWindowTask(16, EachSixtyFrames);
}
void Controller::LoadWindowClasses(const char* filepath) {}
void Controller::SaveWindows(const char* filepath) const {}
void Controller::SaveWindowClasses(const char* filepath) const {}