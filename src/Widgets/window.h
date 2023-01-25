#pragma once

#include "../windowinput.h"

#include <wtypes.h>
#include <string>
#include <memory>
class WindowClass;

struct LoadData {
	std::shared_ptr<WindowClass> window;
	std::wstring id;
	std::wstring parentId;
};

class WindowClass {
private:
	HWND hwnd;
	HINSTANCE hInstance;
	LPCWSTR className;
	TRACKMOUSEEVENT tracker;
	Input input;
	std::string* time_string;
	HFONT hFont;
	HBRUSH hBrush;
public:
	WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP, std::string* time_string = nullptr);

	virtual ~WindowClass();

	HWND GetHwnd() const;

	HFONT GetFont() const;

	HBRUSH GetBrush() const;

	std::string GetTimeString() const;

	Input GetInput() const;

	void SetTimeString(std::string* time_string);

	std::wstring Serialize() const;

	static std::shared_ptr<WindowClass> DeSerialize(const std::wstring& line, const HINSTANCE& hInstance);

	void move(LONG x, LONG y, HWND insertAfter = nullptr);

	virtual bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	void HandleInput(UINT message, WPARAM wParam);
};
