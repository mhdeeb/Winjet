#pragma once

#include "window.h"

LRESULT CALLBACK HandleInput(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace procs {
	void init(HINSTANCE hInstance);
}