#include "controller.h"

//TODO: Save components in file and load them from file on startup
//TODO: Fix bug with moving components
//TODO: Add ability to specify DigitalClock's properties
//TODO: Fix background color
//TODO: parenthood is not implemented yet

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Controller controller(hInstance);
	controller.LoadWindows("save/Windows.txt");
	int exitCode = controller.run();
	return exitCode;
}