#include <Windows.h>
#include <iostream>
#include <string>

#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	registerOverlayWindow(hInstance);
	HWND parent = CreateParentOverlayWindow(hInstance);
	CreateChildOverlayWindow(parent, hInstance, L"Base", 0, 0, 0, 0, true);
	CreateChildOverlayWindow(parent, hInstance, L"Widget", 1500, 20, 400, 40);
	//LoadWindows();
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//SaveWindows();
	return  int(msg.wParam);
}