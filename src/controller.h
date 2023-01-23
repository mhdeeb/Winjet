#pragma once

#include "CallBackTimer.h"

#include <wtypes.h>
#include <string>
#include <vector>
#include <map>

struct LoadData {
	std::wstring className;
	HWND window;
	std::wstring id;
	std::wstring parentId;
};

class Controller {
private:
	MSG msg{};
	HINSTANCE hInstance;
	std::string time_string;
	HFONT hFont;
	HBRUSH hBrush;
	std::map<std::wstring, std::vector<HWND>, std::less<>> classes;
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

std::wstring SerializeWindow(HWND window);
LoadData DeSerializeWindow(const std::wstring& line, HINSTANCE hInstance);
