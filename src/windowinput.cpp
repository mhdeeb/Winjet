#include "windowinput.h"

Input::Input(HWND hwnd) {
	tracker.cbSize = sizeof(tracker);
	tracker.hwndTrack = hwnd;
	tracker.dwFlags = TME_LEAVE;
	tracker.dwHoverTime = HOVER_DEFAULT;
}

void Input::HandleInput(UINT message, WPARAM wParam) {
	switch (message) {
	case WM_MOUSELEAVE:
		mouse.ButtonState.fill(false);
		mouse.inWindow = false;
		return;
	case WM_MOUSEMOVE:
		if (!mouse.inWindow) {
			TrackMouseEvent(&tracker);
			mouse.inWindow = true;
		}
		return;
	case WM_LBUTTONDOWN:
		mouse.SetButtonState(MouseButton::LEFT, true);
		return;
	case WM_LBUTTONUP:
		mouse.SetButtonState(MouseButton::LEFT, false);
		return;
	case WM_RBUTTONDOWN:
		mouse.SetButtonState(MouseButton::RIGHT, true);
		return;
	case WM_RBUTTONUP:
		mouse.SetButtonState(MouseButton::RIGHT, false);
		return;
	case WM_MBUTTONDOWN:
		mouse.SetButtonState(MouseButton::MIDDLE, true);
		return;
	case WM_MBUTTONUP:
		mouse.SetButtonState(MouseButton::MIDDLE, false);
		return;
	case WM_KEYDOWN:
		keyboard.SetKeyState(int(wParam), true);
		return;
	case WM_KEYUP:
		keyboard.SetKeyState(int(wParam), false);
		return;
	}
}