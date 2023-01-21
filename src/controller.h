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
	std::string time_string;
	HFONT hFont;
	HBRUSH hBrush;
	std::map<LPCWSTR, std::vector<HWND>> classes;
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

	HWND AddWindow(LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName, UINT styles, UINT ExStyles, HWND parent);

	void LoadWindows(const char* filepath);

	void SaveWindows(const char* filepath) const;

	friend void EachSixtyFrames(const Controller* cont);

	friend void UpdateTime(Controller* cont);
};