#pragma once
#include <wtypes.h>

#include "Component.h"
#include "Text.h"
#include "../CallBackTimer.h"

class DigitalClock: public Component
{
private:
	Text time_string;
	CallBackTimer time_updater;
public:
	explicit DigitalClock(RECT rect, const paint::Pen& pen, const paint::Brush& brush, const Text& time_string, HWND hwnd);
	~DigitalClock() override;
	void paint(HDC hdc) const override;
	void move(const POINT& point) override;
	void rmove(const POINT& delta) override;
	bool IsPointInComponent(const POINT& point) const override;
	void SetHwnd(HWND hwnd) override;
	void UpdateTime();
	nlohmann::json Serialize() const override;
	static std::shared_ptr<DigitalClock> Deserialize(const nlohmann::json& SerializedDigitalClock, HWND hwnd);
};
