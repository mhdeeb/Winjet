#include "Ball.h"

#include "../includes/json.hpp"

constexpr auto DELTA_TIME = 0.01;
constexpr auto GRAVITY = 100;
constexpr auto RESITUTION_FACTOR = 0.85;
constexpr auto FRICTION_FACTOR = 0.95;
constexpr auto DELTA_SCALE = 4;

Ball::Ball(RECT rect, HWND hwnd, const paint::Pen& Pen, const paint::Brush& Brush) : Component(rect, hwnd, Pen, Brush) {
	time_updater.start(int(DELTA_TIME * 1000), [this]() { UpdatePhysics(); });
}

void Ball::paint(HDC hdc) const {
	RECT rc(rect);
	SelectObject(hdc, GetPen().GetPen());
	SelectObject(hdc, GetBrush().GetBrush());
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

void Ball::UpdatePhysics() {
	RECT rc;
	if (!isSelected) {
		vy += GRAVITY * DELTA_TIME;
		rmove({ int(vx), int(vy) });
		GetClientRect(GetHwnd(), &rc);
		if (rect.left < rc.left) {
			vx = int(abs(vx * RESITUTION_FACTOR));
			vy *= FRICTION_FACTOR;
			rmove({ rc.left - rect.left, 0 });
		} else if (rect.right > rc.right) {
			vx = -int(abs(vx * RESITUTION_FACTOR));
			vy *= FRICTION_FACTOR;
			rmove({ rc.right - rect.right, 0 });
		}
		if (rect.bottom > rc.bottom) {
			vx *= FRICTION_FACTOR;
			vy = -int(abs(vy * RESITUTION_FACTOR));
			rmove({ 0, rc.bottom - rect.bottom });
		} else if (rect.top < rc.top) {
			vx *= FRICTION_FACTOR;
			vy = int(abs(vy * RESITUTION_FACTOR));
			rmove({ 0, rc.top - rect.top });
		}
	}
}

void Ball::rmove(const POINT& delta) {
	Component::rmove(delta);
	if (isSelected) {
		vx = DELTA_SCALE * delta.x;
		vy = DELTA_SCALE * delta.y;
	}
}

nlohmann::json Ball::Serialize() const {
	nlohmann::json j;
	j["Component"] = "Ball";
	j["Rect"] = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	j["Pen"] = pen.Serialize();
	j["Brush"] = brush.Serialize();
	return j;
}

std::shared_ptr<Ball> Ball::Deserialize(const nlohmann::json& serializedText, HWND hwnd) {
	auto rect = serializedText["Rect"].get<std::vector<int>>();
	auto pen = paint::Pen::Deserialize(serializedText["Pen"]);
	auto brush = paint::Brush::Deserialize(serializedText["Brush"]);
	return std::make_shared<Ball>(RECT{ rect[0], rect[1], rect[0] + rect[2], rect[1] + rect[3] }, hwnd, pen, brush);
}