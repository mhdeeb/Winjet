#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Controller controller(hInstance);
	controller.LoadWindowClasses("WindowClasses.txt");
	controller.LoadWindows("Windows.txt");
	int exitCode = controller.run();
	controller.SaveWindowClasses("WindowClasses.txt");
	controller.SaveWindows("Windows.txt");
	return exitCode;
}