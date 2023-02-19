#include "Command.h"
#include "../util.h"
#include "../includes/json.hpp"
#include "../process.h"

#include <fstream>

Command::Command(RECT rect, const paint::Pen& pen, const paint::Brush& brush, const Text& command_string, std::string_view command, int refreshTime, HWND hwnd): Component(rect, hwnd, pen, brush), command_string(command_string), command(command) {
	time_updater.start(refreshTime, [this]() { UpdateCommand(); });
}

void Command::paint(HDC hdc) const {
	FillRect(hdc, &rect, brush.GetBrush());
	command_string.paint(hdc);
}

void Command::UpdateCommand() {
	command_string.SetText(exec(command));
}

Command::~Command() {
	time_updater.stop();
}

void Command::move(const POINT& point) {
	Component::move(point);
	command_string.move(point);
}

void Command::rmove(const POINT& delta) {
	Component::rmove(delta);
	command_string.rmove(delta);
}

bool Command::IsPointInComponent(const POINT& point) const {
	return Component::IsPointInComponent(point) || command_string.IsPointInComponent(point);
}

void Command::SetHwnd(HWND hwnd) {
	Component::SetHwnd(hwnd);
	command_string.SetHwnd(hwnd);
}

nlohmann::json Command::Serialize() const {
	nlohmann::json j;
	j["Component"] = "Command";
	j["Rect"] = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	j["Pen"] = pen.Serialize();
	j["Brush"] = brush.Serialize();
	j["Text"] = command_string.Serialize();
	j["Command"] = command;
	j["RefreshTime"] = time_updater.getInterval();
	return j;
}

std::shared_ptr<Command> Command::Deserialize(const nlohmann::json& SerializedCommand, HWND hwnd) {
	RECT rect;
	rect.left = SerializedCommand["Rect"][0];
	rect.top = SerializedCommand["Rect"][1];
	rect.right = rect.left + SerializedCommand["Rect"][2];
	rect.bottom = rect.top + SerializedCommand["Rect"][3];
	paint::Pen pen = paint::Pen::Deserialize(SerializedCommand["Pen"]);
	paint::Brush brush = paint::Brush::Deserialize(SerializedCommand["Brush"]);
	Text command_string = *Text::Deserialize(SerializedCommand["Text"], hwnd);
	std::string command = SerializedCommand["Command"];
	int refreshTime = SerializedCommand["RefreshTime"];
	return std::make_shared<Command>(rect, pen, brush, command_string, command, refreshTime, hwnd);
}