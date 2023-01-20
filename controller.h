#pragma once
#include "windowinput.h"

std::vector<std::shared_ptr<input>> inputHandlers;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	for (auto i : inputHandlers)
		if (i->hwnd == hwnd)
			return i->HandleInput(hwnd, message, wParam, lParam);
	return DefWindowProc(hwnd, message, wParam, lParam);
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
	wndclass.hbrBackground = 0;
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = L"Overlay";

	RegisterClass(&wndclass);
}

HWND CreateChildOverlayWindow(HWND parent, HINSTANCE hInstance, const wchar_t* name, int iX, int iY, int width, int hight, bool isMaxized = false) {
	HWND hwnd = CreateWindowExW(
		WS_EX_LAYERED,
		L"Overlay",
		name,
		//WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		WS_POPUP | WS_VISIBLE | WS_CHILDWINDOW | (isMaxized ? WS_MAXIMIZE : 0),
		iX,
		iY,
		width,
		hight,
		parent, nullptr, hInstance, nullptr
	);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 1), 0, LWA_COLORKEY);
	inputHandlers.push_back(std::make_shared<input>(hwnd, hInstance));
	return hwnd;
}

HWND CreateParentOverlayWindow(HINSTANCE hInstance) {
	HWND hwnd = CreateWindowExW(
		WS_EX_LAYERED | WS_EX_TRANSPARENT,
		L"Overlay",
		nullptr,
		//WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		WS_MAXIMIZE,
		0,
		0,
		0,
		0,
		HWND_DESKTOP, nullptr, hInstance, nullptr
	);
	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
	//inputHandler = std::make_unique<input>(hwnd, hInstance);
	return hwnd;
}