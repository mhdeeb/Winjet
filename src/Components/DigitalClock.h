#pragma once
#include <wtypes.h>

#include "Component.h"
#include "Text.h"
#include "../CallBackTimer.h"

class DigitalClock : public Component
{
private:
	Text time_string;
	CallBackTimer time_updater;
public:
	explicit DigitalClock(RECT rect, const paint::Pen& pen = paint::Pen(PS_SOLID, 1, RGB(0, 0, 0)), const paint::Brush& brush = paint::Brush(RGB(0, 0, 0)), const Text& time_string = Text({}, L"0000-00-00 00:00:00", nullptr, paint::Color::TRANSPARENTC, { 40, BOLD, MODERN, L"Curier New" }, BS_CENTER), HWND hwnd = nullptr);
	~DigitalClock() override;
	void paint(HDC hdc) const override;
	void move(const POINT& point) override;
	void rmove(const POINT& delta) override;
	void SetHwnd(HWND hwnd) override;
	void UpdateTime();
	//FIX
	nlohmann::json Serialize() const override;
	//FIX
	static std::shared_ptr<DigitalClock> Deserialize(const nlohmann::json& SerializedDigitalClock);
};
