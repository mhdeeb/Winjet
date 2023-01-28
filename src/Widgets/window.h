#pragma once

#include "../windowinput.h"
#include "../Components/Component.h"

#include <wtypes.h>
#include <string>
#include <vector>
#include <memory>

class WindowClass {
private:
	HWND hwnd;
	HINSTANCE hInstance;
	LPCWSTR className;
	TRACKMOUSEEVENT tracker;
	Input input;
	std::vector<std::shared_ptr<Component>> components;
public:
	WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP);

	virtual ~WindowClass();

	HWND GetHwnd() const;

	Input GetInput() const;

	std::wstring Serialize() const;

	static std::shared_ptr<WindowClass> DeSerialize(const std::wstring& line, const HINSTANCE& hInstance, int& parentId);

	void move(LONG x, LONG y, HWND insertAfter = nullptr);

	virtual bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	void HandleInput(UINT message, WPARAM wParam);

	void DrawComponents(const HDC& hdc) const;

	void AddComponent(std::shared_ptr<Component> component);

	void RemoveComponent(std::shared_ptr<Component> component);

	std::shared_ptr<Component> GetComponentAtPoint(const POINT& point) const;
};
