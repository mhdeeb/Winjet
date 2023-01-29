#include "DigitalClock.h"
#include "../util.h"
#include <iostream>

DigitalClock::DigitalClock(RECT rect, const paint::Pen& pen, const paint::Brush& brush, HWND hwnd) : Component(rect, hwnd, pen, brush), time_string(rect, "0000-00-00 00:00:00", hwnd, paint::Color::TRANSPARENTC, { 40, BOLD, MODERN, L"Curier New" }, BS_CENTER) {
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