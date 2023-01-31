#include "controller.h"
#include "includes/json.hpp"

#include <fstream>

Controller::Controller(HINSTANCE HInstance) : hInstance(HInstance) {
	auto style = std::stoul("0x97000000", nullptr, 16);
	auto exStyle = std::stoul("0x8080080", nullptr, 16);
	window = std::make_shared<CanvasWindow>(hInstance,
		0, 0, 0, 0,
		paint::Brush(paint::Color::TRANSPARENTC),
		L"Winjet",
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS,
		style,
		exStyle);
}

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

	for (nlohmann::json data = nlohmann::json::parse(file); auto const& component : data) {
		window->AddComponent(Component::Deserialize(component, window->GetHwnd()));
	}

	file.close();
}

void Controller::SaveData(const char* filepath) const {
	std::ofstream file(filepath);
	nlohmann::json j;
	for (auto const& component : *window->GetComponents()) {
		if (component)
			j.push_back(component->Serialize());
	}
	file << j.dump(4);
	file.close();
}