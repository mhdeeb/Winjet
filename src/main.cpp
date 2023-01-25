#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Controller controller(hInstance);
	controller.LoadWindows("save/Windows.txt");
	int exitCode = controller.run();
	//controller.SaveWindows("save/Windows");
	return exitCode;
}