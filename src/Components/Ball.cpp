#include "Ball.h"

#include "../includes/json.hpp"

constexpr auto DELTA_TIME = 0.01;
constexpr auto GRAVITY = 60;
constexpr auto RESITUTION_FACTOR = 0.9;

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
			rmove({ rc.left - rect.left, 0 });
		} else if (rect.right > rc.right) {
			vx = -int(abs(vx * RESITUTION_FACTOR));
			rmove({ rc.right - rect.right, 0 });
		}
		if (rect.bottom > rc.bottom - 50) {
			vy = -int(abs(vy * RESITUTION_FACTOR));
			rmove({ 0, rc.bottom - rect.bottom - 50 });
		} else if (rect.top < rc.top) {
			vy = int(abs(vy * RESITUTION_FACTOR));
			rmove({ 0, rc.top - rect.top });
		}
	}
}

void Ball::rmove(const POINT& delta) {
	Component::rmove(delta);
	if (isSelected) {
		vx = 3 * delta.x;
		vy = 3 * delta.y;
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