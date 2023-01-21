#pragma once

#include <wtypes.h>

constexpr auto HIDDENCLASS = L"HiddenWindow";
constexpr auto CANVASCLASS = L"CanvasWindow";

void RegisterWindowClass(HINSTANCE hInstance, LPCWSTR className, WNDPROC wndProc, UINT style = CS_HREDRAW | CS_VREDRAW);

HWND CreateWidgetWindow(LPCWSTR className, HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP);

HWND CreateOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, int x, int y, int width, int height, LPCWSTR className = nullptr, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP);

HWND CreateHiddenWindow(HINSTANCE hInstance);

HWND CreateCanvasOverlayWindow(HINSTANCE hInstance, HWND parent);

HWND CreateChildOverlayWindow(HINSTANCE hInstance, LPCWSTR windowName, LPCWSTR className, HWND parent, int x, int y, int width, int height, UINT styles = NULL);

void move(HWND hwnd, LONG x, LONG y, HWND insertAfter = nullptr);
