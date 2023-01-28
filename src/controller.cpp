#include "controller.h"
#include "messages.h"
#include "Widgets/canvas.h"
#include "Widgets/widget.h"

#include <fstream>
#include <sstream>

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	AddTask(2000, [this]() { AutoSave(); });
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
	windows[window->GetHwnd()] = window;
}

void Controller::LoadWindows(const char* filepath) {
	std::wifstream file(filepath);
	std::vector<HWND> handles;

	while (!file.eof()) {
		std::wstring winString;
		std::getline(file, winString);
		if (winString.empty()) {
			continue;
		}
		int parentId;
		auto window = WindowClass::DeSerialize(winString, hInstance, parentId);
		handles.push_back(window->GetHwnd());
		if (parentId != -1)
			SetParent(window->GetHwnd(), handles[parentId]);
		windows[window->GetHwnd()] = window;
	}
	file.close();
}

void Controller::SaveWindows(const char* filepath) const {
	std::wstringstream ss;
	for (auto& [hwnd, window] : windows) {
		if (window) {
			ss << window->Serialize();
			HWND parent = GetParent(hwnd);
			if (parent && windows.contains(parent)) {
				ss << ' ' << distance(windows.begin(), windows.find(parent)) << std::endl;
			} else {
				ss << " -1\n";
			}
		}
	}
	std::wofstream file(filepath);
	file << ss.str();
	file.close();
}

void Controller::AutoSave() const {
	//SaveWindows("save/Windows.txt");
}