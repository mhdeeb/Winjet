#include "controller.h"
#include "includes/json.hpp"

#include <fstream>

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
	std::fstream file(filepath);

	for (nlohmann::json data = nlohmann::json::parse(file); auto & component : data) {
		if (component["Component"] == "Window") {
			if (component["ClassName"] == "CanvasWindow")
				window = CanvasWindow::Deserialize(component, hInstance);
		} else
			window->AddComponent(Component::Deserialize(component, window->GetHwnd()));
	}

	file.close();
}

void Controller::SaveData(const char* filepath) const {
	std::ofstream file(filepath);
	nlohmann::json j;
	j.push_back(window->Serialize());
	for (auto const& component : *window->GetComponents()) {
		if (component)
			j.push_back(component->Serialize());
	}
	file << j.dump(4);
	file.close();
}

void Controller::AutoSave() const {
	SaveData("save/data.json");
}