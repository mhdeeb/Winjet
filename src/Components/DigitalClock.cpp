#include "DigitalClock.h"
#include "../util.h"
#include "../includes/json.hpp"

#include <fstream>

DigitalClock::DigitalClock(RECT rect, const paint::Pen& pen, const paint::Brush& brush, const Text& time_string, HWND hwnd): Component(rect, hwnd, pen, brush), time_string(time_string) {
	time_updater.start(16, [this]() { UpdateTime(); });
}

void DigitalClock::paint(HDC hdc) const {
	FillRect(hdc, &rect, brush.GetBrush());
	time_string.paint(hdc);
}

void DigitalClock::UpdateTime() {
	time_string.SetText(return_current_time_and_date());
}

DigitalClock::~DigitalClock() {
	time_updater.stop();
}

void DigitalClock::move(const POINT& point) {
	Component::move(point);
	time_string.move(point);
}

void DigitalClock::rmove(const POINT& delta) {
	Component::rmove(delta);
	time_string.rmove(delta);
}

bool DigitalClock::IsPointInComponent(const POINT& point) const {
	return Component::IsPointInComponent(point) || time_string.IsPointInComponent(point);
}

void DigitalClock::SetHwnd(HWND hwnd) {
	Component::SetHwnd(hwnd);
	time_string.SetHwnd(hwnd);
}

nlohmann::json DigitalClock::Serialize() const {
	nlohmann::json j;
	j["Component"] = "DigitalClock";
	j["Rect"] = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	j["Pen"] = pen.Serialize();
	j["Brush"] = brush.Serialize();
	j["Text"] = time_string.Serialize();
	return j;
}

std::shared_ptr<DigitalClock> DigitalClock::Deserialize(const nlohmann::json& SerializedDigitalClock, HWND hwnd) {
	RECT rect;
	rect.left = SerializedDigitalClock["Rect"][0];
	rect.top = SerializedDigitalClock["Rect"][1];
	rect.right = rect.left + SerializedDigitalClock["Rect"][2];
	rect.bottom = rect.top + SerializedDigitalClock["Rect"][3];
	paint::Pen pen = paint::Pen::Deserialize(SerializedDigitalClock["Pen"]);
	paint::Brush brush = paint::Brush::Deserialize(SerializedDigitalClock["Brush"]);
	Text text = *Text::Deserialize(SerializedDigitalClock["Text"], hwnd);
	return std::make_shared<DigitalClock>(rect, pen, brush, text, hwnd);
}