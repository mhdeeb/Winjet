#pragma once
#include <wtypes.h>

#include "Component.h"
#include "Text.h"
#include "../CallBackTimer.h"

class Command: public Component
{
private:
	Text command_string;
	CallBackTimer time_updater;
	std::string command;
public:
	explicit Command(RECT rect, const paint::Pen& pen, const paint::Brush& brush, const Text& command_string, std::string_view command, int refreshTime, HWND hwnd);
	~Command() override;
	void paint(HDC hdc) const override;
	void move(const POINT& point) override;
	void rmove(const POINT& delta) override;
	bool IsPointInComponent(const POINT& point) const override;
	void SetHwnd(HWND hwnd) override;
	void UpdateCommand();
	nlohmann::json Serialize() const override;
	static std::shared_ptr<Command> Deserialize(const nlohmann::json& SerializedDigitalClock, HWND hwnd);
};
