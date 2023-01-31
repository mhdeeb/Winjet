#include "Component.h"
#include "Text.h"
#include "DigitalClock.h"

#include "../includes/json.hpp"

int Component::idCounter = 0;

Component::Component(RECT rect, HWND hwnd, const paint::Pen& pen, const paint::Brush& brush) : rect(rect), pen(pen), brush(brush), hwnd(hwnd) { id = idCounter; idCounter++; }

Component::~Component() { idCounter--; }

void Component::SetPen(const paint::Pen& pen) { this->pen = pen; Invalidate(); }

void Component::SetBrush(const paint::Brush& brush) { this->brush = brush; Invalidate(); }

paint::Pen Component::GetPen() const { return pen; }

paint::Brush Component::GetBrush() const { return brush; }

void Component::SetHwnd(HWND hwnd) { this->hwnd = hwnd; }

HWND Component::GetHwnd() const { return hwnd; }

void Component::SetRect(RECT rect) { this->rect = rect; Invalidate(); }

RECT Component::GetRect() const { return rect; }

//FIX
std::shared_ptr<Component> Component::Deserialize(const nlohmann::json& data)
{
	std::string type = data["Component"];
	if (type == "DigitalClock") {
		return DigitalClock::Deserialize(data);
	} else if (type == "Text") {
		return Text::Deserialize(data);
	} else {
		return nullptr;
	}
}

void Component::Invalidate() const { InvalidateRect(hwnd, &rect, false); }

bool Component::IsPointInComponent(const POINT& point) const {
	return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
}

void Component::move(const POINT& point) {
	RECT r = rect;
	rect.right += point.x - rect.left;
	rect.bottom += point.y - rect.top;
	rect.left = point.x;
	rect.top = point.y;
	r = { min(rect.left, r.left), min(rect.top, r.top), max(rect.right, r.right), max(rect.bottom, r.bottom) };
	InvalidateRect(hwnd, &r, false);
}

void Component::rmove(const POINT& delta) {
	RECT r = rect;
	rect.left += delta.x;
	rect.right += delta.x;
	rect.top += delta.y;
	rect.bottom += delta.y;
	r = { min(rect.left, r.left), min(rect.top, r.top), max(rect.right, r.right), max(rect.bottom, r.bottom) };
	InvalidateRect(hwnd, &r, false);
}