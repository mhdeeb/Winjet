#pragma once
#include "windowinput.h"

std::unique_ptr<input> inputHandler = nullptr;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (inputHandler)
		return inputHandler->HandleInput(hwnd, message, wParam, lParam);
	else return DefWindowProc(hwnd, message, wParam, lParam);
}

void registerOverlayWindow(HINSTANCE hInstance) {
	WNDCLASS wndclass{};

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(nullptr, IDI_ASTERISK);
	wndclass.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = L"Overlay";

	RegisterClass(&wndclass);
}

HWND CreateOverlayWindow(HINSTANCE hInstance) {
	HWND hwnd = GetDesktopWindow();

	RECT desktop;
	GetWindowRect(hwnd, &desktop);

	hwnd = CreateWindowExW(
		WS_EX_LAYERED,
		L"Overlay",
		nullptr,
		WS_POPUP | WS_VISIBLE,
		desktop.left + 100,
		desktop.top + 100,
		100,
		100,
		hwnd, nullptr, hInstance, nullptr
	);

	SetLayeredWindowAttributes(hwnd, 0, 127, LWA_ALPHA);
	inputHandler = std::make_unique<input>(hwnd, hInstance);
	return hwnd;
}