#include "Component.h"

int Component::idCounter = 0;

Component::Component(RECT rect, const paint::Pen& pen, const paint::Brush& brush) : rect(rect), pen(pen), brush(brush) { id = idCounter; idCounter++; }

Component::~Component() { idCounter--; }

void Component::SetPen(const paint::Pen& pen) { this->pen = pen; }
void Component::SetBrush(const paint::Brush& brush) { this->brush = brush; }

bool Component::IsPointInComponent(const POINT& point) const {
	return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
}

paint::Pen Component::GetPen() const { return pen; }
paint::Brush Component::GetBrush() const { return brush; }

void Component::move(const POINT& point) {
	rect.right += point.x - rect.left;
	rect.bottom += point.y - rect.top;
	rect.left = point.x;
	rect.top = point.y;
}

void Component::rmove(const POINT& delta) {
	rect.left += delta.x;
	rect.right += delta.x;
	rect.top += delta.y;
	rect.bottom += delta.y;
}