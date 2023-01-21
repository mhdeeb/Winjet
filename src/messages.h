#pragma once

#include <Windows.h>

#define WM_FRAME (WM_USER + 1)
#define WM_60_FRAMES (WM_USER + 2)

bool isMouseEvent(UINT message);

bool isKeyboardEvent(UINT message);

void Log(UINT message, WPARAM wParam);