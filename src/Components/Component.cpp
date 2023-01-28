#include "Component.h"

int Component::idCounter = 0;

Component::Component(RECT rect, const paint::Pen& pen, const paint::Brush& brush): rect(rect), pen(pen), brush(brush) { id = idCounter; idCounter++; }

Component::~Component() { idCounter--; }

void Component::SetPen(const paint::Pen& pen) { this->pen = pen; }
void Component::SetBrush(const paint::Brush& brush) { this->brush = brush; }

bool Component::IsPointInComponent(const POINT& p) const {
	return p.x >= rect.left && p.x <= rect.right && p.y >= rect.top && p.y <= rect.bottom;
}

paint::Pen Component::GetPen() const { return pen; }
paint::Brush Component::GetBrush() const { return brush; }

void Component::move(const POINT& p) {
	rect.left += p.x;
	rect.right += p.x;
	rect.top += p.y;
	rect.bottom += p.y;
}