#pragma once

#include "CallBackTimer.h"

#include <wtypes.h>
#include <string>
#include <vector>
#include <map>

class Controller {
private:
	MSG msg{};
	HINSTANCE hInstance;
	HWND base;
	HWND canvas;
	std::string time_string;
	HFONT hFont;
	HBRUSH hBrush;
	std::map<LPCWSTR, WNDPROC> classes;
	std::vector<HWND> windows;
	TimerQueue timers;
	int timerCount = 0;
public:
	explicit Controller(HINSTANCE HInstance);

	~Controller();

	int run();

	void UpdateTime();

	template<typename _FUNC>
	void AddWindowTask(int interval, _FUNC func) {
		for (auto window : windows) {
			timers.add(timerCount++, interval, func, window);
		}
	}

	template<typename _FUNC, typename ... _ARGS>
	void AddTask(int interval, _FUNC func, _ARGS...args) {
		timers.add(timerCount++, interval, func, args...);
	}

	void LoadWindows(const char* filepath);
	void LoadWindowClasses(const char* filepath);
	void SaveWindows(const char* filepath) const;
	void SaveWindowClasses(const char* filepath) const;
};