#pragma once

#include "CallBackTimer.h"
#include "Widgets/canvas.h"

#include <string>

class Controller {
private:
	HINSTANCE hInstance;
	std::shared_ptr<CanvasWindow> window;
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
