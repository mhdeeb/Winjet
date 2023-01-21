#pragma once

#include <wtypes.h>

void RegisterWindowClass(HINSTANCE hInstance, LPCWSTR className, WNDPROC wndProc, UINT style = CS_HREDRAW | CS_VREDRAW);

HWND CreateWidgetWindow(LPCWSTR className, HINSTANCE hInstance, int x, int y, int width, int height, LPCWSTR windowName = nullptr, UINT styles = NULL, UINT ExStyles = NULL, HWND parent = HWND_DESKTOP);

void move(HWND hwnd, LONG x, LONG y, HWND insertAfter = nullptr);
