#pragma once

#include <wtypes.h>
#include <CommCtrl.h>
#include <array>

enum class MouseButton {
	NONE,
	LEFT,
	RIGHT,
	MIDDLE
};

struct MouseInput {
	POINT p = {0, 0};
	POINT dp = {0, 0};
	bool inWindow = false;
	std::array<bool, 4> ButtonState{};
	std::array<bool, 4> PrevButtonState{};

	void SetButtonState(MouseButton button, bool state) {
		PrevButtonState[int(button)] = ButtonState[int(button)];
		ButtonState[int(button)] = state;
	}

	void SetMousePosition(POINT point) {
		dp = {point.x - p.x, point.y - p.y};
		p = point;
	}

	POINT GetMousePosition() const {
		return p;
	}

	POINT GetDeltaMousePosition() const {
		return dp;
	}

	bool isButtonPressed(MouseButton button) const {
		return ButtonState[int(button)] && !PrevButtonState[int(button)];
	}

	bool isButtonDepressed(MouseButton button) const {
		return PrevButtonState[int(button)] && !ButtonState[int(button)];
	}

	bool isButtonDown(MouseButton button) const {
		return ButtonState[int(button)];
	}
};

struct KeyboardInput {
	std::array<bool, 256> keyState{};
	std::array<bool, 256> PrevkeyState{};

	void SetKeyState(int virtualKey, bool isDown) {
		PrevkeyState[virtualKey] = keyState[virtualKey];
		keyState[virtualKey] = isDown;
	}

	std::array<bool, 256> GetKeyState() const {
		return keyState;
	}

	bool isKeyPressed(int virtualKey) const {
		return keyState[virtualKey] && !PrevkeyState[virtualKey];
	}

	bool isKeyDepressed(int virtualKey) const {
		return PrevkeyState[virtualKey] && !keyState[virtualKey];
	}

	bool isKeyDown(int virtualKey) const {
		return keyState[virtualKey];
	}
};

struct Input {
	MouseInput mouse;
	KeyboardInput keyboard;
};
