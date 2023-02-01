#pragma once

#include <Windows.h>

#define WM_MYCONST WM_USER + 300
#define CM_ABOUT (WM_MYCONST + 1)
#define CM_EXIT (WM_MYCONST + 2)
#define CM_TRAY (WM_MYCONST + 3)

bool isMouseEvent(UINT message);

bool isKeyboardEvent(UINT message);

void Log(UINT message, WPARAM wParam);