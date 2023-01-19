#pragma once

#include <Windows.h>
#include <dwmapi.h>
//#include <Uxtheme.h>

enum class PreferredAppMode
{
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};

void CheckDarkMode(HWND hwnd) {
	using fnShouldAppsUseDarkMode = bool (WINAPI*)();
	using fnAllowDarkModeForWindow = bool (WINAPI*)(HWND hWnd, bool allow);
	using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);
	HMODULE hUxtheme = LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	fnShouldAppsUseDarkMode ShouldAppsUseDarkMode = (fnShouldAppsUseDarkMode)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(132));
	fnAllowDarkModeForWindow AllowDarkModeForWindow = (fnAllowDarkModeForWindow)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133));
	fnSetPreferredAppMode SetPreferredAppMode = (fnSetPreferredAppMode)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
	FreeLibrary(hUxtheme);
	SetPreferredAppMode(PreferredAppMode::AllowDark);
	AllowDarkModeForWindow(hwnd, true);
	BOOL value = true;
	DwmSetWindowAttribute(hwnd, 20, &value, sizeof(value));
	SendMessage(hwnd, WM_THEMECHANGED, 0, 0);
}

void move(HWND hwnd, LONG x, LONG y) {
	RECT rect;
	GetClientRect(hwnd, &rect);
	SetWindowPos(hwnd, HWND_BOTTOM, rect.left + x, rect.top + y, rect.right - rect.left, rect.bottom - rect.top, 0);
}
