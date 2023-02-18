#include "controller.h"
#include "includes/json.hpp"

#include <fstream>

Controller::Controller(HINSTANCE HInstance): hInstance(HInstance) {
	RECT desktop_rect;
	GetWindowRect(GetDesktopWindow(), &desktop_rect);
	window = std::make_shared<CanvasWindow>(hInstance,
		0, 0, desktop_rect.right, desktop_rect.bottom - 1,
		paint::Brush(paint::Color::TRANSPARENTC),
		L"Winjet",
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS,
		WS_POPUP | WS_VISIBLE,
		WS_EX_LAYERED | WS_EX_TOOLWINDOW);
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