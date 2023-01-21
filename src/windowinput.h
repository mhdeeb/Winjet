#pragma once

#include "CallBackTimer.h"

#include <string>
#include <wtypes.h>
#include <CommCtrl.h>

struct mouseInput {
	POINT p = {0, 0};
	POINT delta = {0, 0};
	bool isLeftDown = false;
	bool isRightDown = false;
	bool isMiddleDown = false;
	bool inWindow = false;
	TRACKMOUSEEVENT tracker;
	explicit mouseInput(HWND hwnd) {
		tracker.cbSize = sizeof(tracker);
		tracker.hwndTrack = hwnd;
		tracker.dwFlags = TME_LEAVE;
		tracker.dwHoverTime = HOVER_DEFAULT;
	}
};

struct keyboardInput {
	bool isKeyDown = false;
	WPARAM charCode = 0;
};

LRESULT HandleMouse(mouseInput& mouse, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT HandleKeyboard(keyboardInput& keyboard, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void HandlePaint(HWND hwnd);

std::string return_current_time_and_date();

struct input {
	HINSTANCE hInstance;
	mouseInput mouse;
	keyboardInput keyboard;
	CallBackTimer timer;
	CallBackTimer timer2;
	HWND hwnd;

	input(HWND hwnd, HINSTANCE hInstance);

	~input();

	LRESULT HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
