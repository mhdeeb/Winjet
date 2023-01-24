#pragma once

#include "windowinput.h"

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

	static LoadData DeSerialize(const std::wstring& line, const HINSTANCE& hInstance);

	void move(LONG x, LONG y, HWND insertAfter = nullptr);

	virtual bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	void HandleInput(UINT message, WPARAM wParam);
};

class HiddenWindow: public WindowClass {
public:
	HiddenWindow(HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP, UINT ExStyles = WS_EX_NOACTIVATE | WS_EX_TRANSPARENT, HWND parent = HWND_DESKTOP, std::string* time_string = nullptr);
	bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) override;
};

class CanvasWindow: public WindowClass {
public:
	CanvasWindow(HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CHILD, UINT ExStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT, HWND parent = HWND_DESKTOP, std::string* time_string = nullptr);
	bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) override;
};

class WidgetWindow: public WindowClass {
public:
	WidgetWindow(HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = WS_POPUP | WS_VISIBLE | WS_CHILD, UINT ExStyles = WS_EX_LAYERED, HWND parent = HWND_DESKTOP, std::string* time_string = nullptr);
	bool WinProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
};
