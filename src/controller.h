#pragma once

#include "CallBackTimer.h"
#include "Widgets/canvas.h"

#include <string>

class Controller {
private:
	HINSTANCE hInstance;
	std::unique_ptr<CanvasWindow> window = std::make_unique<CanvasWindow>(hInstance, 0, 0, 0, 0, paint::Brush(paint::TRANSPARENTC), L"Winjet", NULL, 0x97000000L, 0x80c0000L, HWND_DESKTOP);
	TimerQueue timers;
	int timerCount = 0;
public:
	explicit Controller(HINSTANCE HInstance);

	~Controller();

	int run();

	template<typename _FUNC, typename ... _ARGS>
	void AddTask(int interval, _FUNC func, _ARGS...args) {
		timers.add(timerCount++, interval, func, args...);
	}

	void LoadData(const char* filepath);

	void SaveData(const char* filepath) const;

	void AutoSave() const;
};
