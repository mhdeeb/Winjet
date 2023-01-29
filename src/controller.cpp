#include "controller.h"
#include "Widgets/canvas.h"

#include <fstream>
#include <sstream>

Controller::Controller(HINSTANCE HInstance) : hInstance(HInstance) {}

Controller::~Controller() {
	timers.stopAll();
	DestroyWindow(window->GetHwnd());
}

int Controller::run() {
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		window->HandleInput(msg.message, msg.wParam);
		window->WinProc(msg.message, msg.wParam, msg.lParam);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);
}

void Controller::LoadData(const char* filepath) {
	std::wifstream file(filepath);

	while (!file.eof()) {
		std::wstring line;
		std::getline(file, line, L';');
		std::wstringstream ss(line);
		COLORREF color;
		ss >> color;
		window->SetBrush(paint::Brush(color));
		while (!ss.eof()) {
			std::wstring widgetLine;
			std::getline(ss, widgetLine, L';');
			std::shared_ptr<Component> widget = Component::DeSerialize(widgetLine, window->GetHwnd());
			if (widget) window->AddComponent(widget);
		}
	}

	file.close();
}

void Controller::SaveData(const char* filepath) const {
	std::wofstream file(filepath);
	file << window->Serialize();
	file.close();
}

void Controller::AutoSave() const {
	SaveData("save/data.txt");
}