#pragma once

#include "window.h"

class CanvasWindow : public WindowClass {
private:
	paint::Brush brush;
public:
	CanvasWindow(HINSTANCE hInstance, int x, int y, int width, int height, const paint::Brush& brush, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CHILD, UINT ExStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT, HWND parent = HWND_DESKTOP);
	bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) override;
	//FIX
	nlohmann::json Serialize() const override;
	//FIX
	static std::shared_ptr<CanvasWindow> Deserialize(const nlohmann::json& data, HINSTANCE hInstance);
};