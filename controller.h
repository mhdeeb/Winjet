#pragma once
#include "windowinput.h"
#include <map>

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, message, wParam, lParam);
}

class Controller {
private:
	MSG msg;
	HINSTANCE hInstance;
	std::map<const char*, WNDPROC> classes;
	std::vector<HWND> windows;
public:
	Controller(HINSTANCE hInstance): hInstance(hInstance) {}

	~Controller() {}

	int run() {
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return int(msg.wParam);
	}

	void LoadWindows(const char* filepath) {}
	void LoadWindowClasses(const char* filepath) {}
	void SaveWindows(const char* filepath) const {}
	void SaveWindowClasses(const char* filepath) const {}
};