#include "controller.h"
#include "messages.h"
#include "util.h"
#include "Widgets/canvas.h"
#include "Widgets/widget.h"

#include <fstream>

void EachSixtyFrames(const Controller* cont) {
	for (auto& [hwnd, window] : cont->windows) {
		PostMessage(hwnd, WM_60_FRAMES, 0, 0);
	}
}

void UpdateTime(Controller* cont) {
	cont->time_string = return_current_time_and_date();
	cont->SaveWindows("save/Windows");
}

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	AddTask(1000, UpdateTime, this);
	AddTask(16, EachSixtyFrames, this);
}

Controller::~Controller() {
	timers.stopAll();
	for (auto const& [hwnd, window] : windows) {
		DestroyWindow(hwnd);
	}
	windows.clear();
}

int Controller::run() {
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMsg(&msg);
	}
	return int(msg.wParam);
}

LRESULT Controller::DispatchMsg(const MSG* msg) {
	if (windows[msg->hwnd]) {
		windows[msg->hwnd]->HandleInput(msg->message, msg->wParam);
		windows[msg->hwnd]->WinProc(msg->message, msg->wParam, msg->lParam);
	}
	return DispatchMessage(msg);
}

void Controller::AddWindow(std::shared_ptr<WindowClass> window) {
	window->SetTimeString(&time_string);
	windows[window->GetHwnd()] = window;
}

void Controller::LoadWindows(const char* filepath) {
	std::wifstream file(filepath);
	while (!file.eof()) {
		std::wstring winString;
		std::getline(file, winString);
		if (winString.empty()) {
			continue;
		}
		auto window = WindowClass::DeSerialize(winString, hInstance);
		window->SetTimeString(&time_string);
		windows[window->GetHwnd()] = window;
	}
	file.close();
}

void Controller::SaveWindows(const char* filepath) const {
	std::wstringstream ss;
	for (auto& [hwnd, window] : windows) {
		if (window)
			ss << window->Serialize();
	}
	std::wofstream file(filepath);
	file << ss.str();
	file.close();
}