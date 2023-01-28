#include "DigitalClock.h"
#include "../util.h"
#include <iostream>

DigitalClock::DigitalClock(RECT rect, const paint::Pen& pen, const paint::Brush& brush): Component(rect, pen, brush), time_string(rect, "0000-00-00 00:00:00", RGB(255, 0, 0), {40, BOLD, MODERN, L"Curier New"}, BS_CENTER) {
	time_updater.start(1000, [this]() { UpdateTime(); });
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

void DigitalClock::move(const POINT& delta) {
	Component::move(delta);
	time_string.move(delta);
}