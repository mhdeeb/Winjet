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
	std::shared_ptr<Component> SelectedComponent = nullptr;
	paint::Brush brush;
public:
	WindowClass(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, const paint::Brush& brush, LPCWSTR windowName = nullptr, UINT classStyle = CS_HREDRAW | CS_VREDRAW, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP);

	virtual ~WindowClass();

	HWND GetHwnd() const;

	Input GetInput() const;

	void move(LONG x, LONG y, HWND insertAfter = nullptr);

	virtual bool WinProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	void HandleInput(UINT message, WPARAM wParam);

	void DrawComponents(const HDC& hdc) const;

	void AddComponent(std::shared_ptr<Component> component);

	void RemoveComponent(std::shared_ptr<Component> component);

	std::vector<std::shared_ptr<Component>>* GetComponents();

	void SelectComponentAtPoint(const POINT& point);

	void ReleaseSelectedComponent();

	std::shared_ptr<Component> GetSelectedComponent() const;

	std::shared_ptr<Component> GetComponentAtPoint(const POINT& point) const;

	void SetBrush(const paint::Brush& brush);

	paint::Brush GetBrush() const;

	//FIX
	virtual nlohmann::json Serialize() const = 0;
};
