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
	wndclass.hIcon = LoadIcon(nullptr, IDI_INFORMATION);
	wndclass.hCursor = LoadCursor(nullptr, IDC_HAND);
	wndclass.hbrBackground = CreateSolidBrush(RGB(100, 0, 100));
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = L"Overlay";

	RegisterClass(&wndclass);
}

HWND CreateOverlayWindow(HINSTANCE hInstance) {
	HWND hwnd = CreateWindowExW(
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		L"Overlay",
		L"Widget",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100,
		100,
		200,
		200,
		HWND_DESKTOP, nullptr, hInstance, nullptr
	);
	SetLayeredWindowAttributes(hwnd, RGB(100, 100, 100), 127, LWA_ALPHA);
	inputHandler = std::make_unique<input>(hwnd, hInstance);
	return hwnd;
}