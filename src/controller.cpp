#include "controller.h"
#include "windowinput.h"
#include "messages.h"
#include "paint.h"
#include "util.h"
#include "procs.h"

#include <fstream>

void EachSixtyFrames(const Controller* cont) {
	for (auto& [Class, windows] : cont->classes) {
		for (auto window : windows) {
			PostMessage(window, WM_60_FRAMES, 0, 0);
		}
	}
}

void UpdateTime(Controller* cont) {
	cont->time_string = return_current_time_and_date();
}

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	SetFont(hFont, 40, BOLD, MODERN, L"Curier New");
	hBrush = CreateSolidBrush(TRANSPARENT);

	AddTask(1000, UpdateTime, this);
	AddTask(16, EachSixtyFrames, this);

	procs::init(HInstance);
}

Controller::~Controller() {
	timers.stopAll();

	for (auto [Class, windows] : classes) {
		for (auto window : windows) {
			DestroyWindow(window);
		}
		windows.clear();
		UnregisterClass(Class.c_str(), hInstance);
	}
	classes.clear();

	DeleteObject(hBrush);
	DeleteObject(hFont);
}

int Controller::run() {
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);
}

HWND Controller::AddWindow(LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName, UINT styles, UINT ExStyles, HWND parent) {
	HWND window = CreateWidgetWindow(
		className,
		hInstance,
		x, y, width, height,
		windowName,
		styles,
		ExStyles,
		parent);
	if (ExStyles & WS_EX_LAYERED) {
		SetLayeredWindowAttributes(window, TRANSPARENT, NULL, LWA_COLORKEY);
	}
	classes[className].push_back(window);
	return window;
}

void Controller::LoadWindows(const char* filepath) {
	//HWND base = AddWindow(
	//	L"HiddenWindow",
	//	0, 0, 0, 0,
	//	L"Base",
	//	WS_POPUP,
	//	WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
	//	HWND_DESKTOP
	//);
	//HWND canvas = AddWindow(
	//	L"CanvasWindow",
	//	0, 0, 0, 0,
	//	L"Canvas",
	//	WS_POPUP | WS_MAXIMIZE | WS_VISIBLE,
	//	WS_EX_LAYERED | WS_EX_TRANSPARENT,
	//	base
	//);
	//AddWindow(
	//	L"WidgetWindow",
	//	0, 0, 100, 100,
	//	L"Test",
	//	WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CHILD,
	//	WS_EX_LAYERED | WS_EX_TRANSPARENT,
	//	canvas
	//);
	std::map<std::wstring, HWND, std::less<>> windows;
	std::wifstream file(filepath);
	while (!file.eof()) {
		std::wstring winString;
		std::getline(file, winString);
		if (winString.empty()) {
			continue;
		}
		auto [className, window, id, parentId] = DeSerializeWindow(winString, hInstance);
		windows[id] = window;
		if (windows.contains(parentId)) {
			SetParent(window, windows[parentId]);
		}
		classes[className].push_back(window);
	}
	file.close();
}

void Controller::SaveWindows(const char* filepath) const {
	std::wstringstream ss;
	for (auto& [Class, windows] : classes) {
		for (auto window : windows) {
			ss << SerializeWindow(window);
		}
	}
	std::wofstream file(filepath);
	file << ss.str();
	file.close();
}

std::wstring SerializeWindow(HWND window) {
	WCHAR windowName[256];
	GetWindowText(window, windowName, 256);
	RECT rect;
	GetWindowRect(window, &rect);
	WCHAR className[256];
	GetClassName(window, className, 256);
	UINT styles = GetWindowLong(window, GWL_STYLE);
	UINT ExStyles = GetWindowLong(window, GWL_EXSTYLE);
	const HWND& id = window;
	const HWND& parentId = GetParent(window);

	std::wstringstream ss;
	ss << windowName << ' ' << rect.left << ' ' << rect.top << ' ' << rect.right << ' ' << rect.bottom << ' ' << className << ' ' << styles << ' ' << ExStyles << ' ' << id << ' ' << parentId << std::endl;
	return ss.str();
}

LoadData DeSerializeWindow(const std::wstring& line, HINSTANCE hInstance) {
	std::wstringstream ss(line);
	std::wstring windowName;
	ss >> windowName;
	RECT rect;
	ss >> rect.left >> rect.top >> rect.right >> rect.bottom;
	std::wstring className;
	ss >> className;
	UINT styles;
	ss >> styles;
	UINT ExStyles;
	ss >> ExStyles;
	std::wstring id;
	ss >> id;
	std::wstring parentId;
	ss >> parentId;

	const HWND& window = CreateWidgetWindow(
		className.c_str(),
		hInstance,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		windowName.c_str(),
		styles,
		ExStyles,
		HWND_DESKTOP
	);

	return LoadData(className, window, id, parentId);
}