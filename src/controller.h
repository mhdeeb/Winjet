#pragma once

#include "CallBackTimer.h"
#include "Widgets/window.h"

#include <wtypes.h>
#include <string>
#include <vector>
#include <map>

class Controller {
private:
	HINSTANCE hInstance;
	std::string time_string;
	std::map<HWND, std::shared_ptr<WindowClass>> windows;
	TimerQueue timers;
	int timerCount = 0;
public:
	explicit Controller(HINSTANCE HInstance);

	~Controller();

	int run();

	LRESULT DispatchMsg(const MSG* msg);

	template<typename _FUNC, typename ... _ARGS>
	void AddTask(int interval, _FUNC func, _ARGS...args) {
		timers.add(timerCount++, interval, func, args...);
	}

	void AddWindow(std::shared_ptr<WindowClass> window);

	void LoadWindows(const char* filepath);

	void SaveWindows(const char* filepath) const;

	friend void EachSixtyFrames(const Controller* cont);

	friend void UpdateTime(Controller* cont);
};
