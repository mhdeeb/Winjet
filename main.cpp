#include <Windows.h>
#include <iostream>
#include <string>

#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	registerOverlayWindow(hInstance);
	CreateOverlayWindow(hInstance);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return  int(msg.wParam);
}