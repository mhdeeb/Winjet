#pragma once

#include "Component.h"
#include "../CallBackTimer.h"

class Ball: public Component
{
private:
	double vx = 0;
	double vy = 0;
	CallBackTimer time_updater;
public:
	Ball(RECT rect, HWND hwnd, const paint::Pen& Pen, const paint::Brush& Brush);
	void paint(HDC hdc) const override;
	void UpdatePhysics();
	void rmove(const POINT& delta) override;
	nlohmann::json Serialize() const override;
	static std::shared_ptr<Ball> Deserialize(const nlohmann::json& serializedText, HWND hwnd);
};
