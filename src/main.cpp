#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Controller controller(hInstance);
	controller.LoadWindows("save/Windows.txt");
	controller.AddTask(2000, [](Controller const* cont) { cont->AutoSave(); }, &controller);
	int exitCode = controller.run();
	return exitCode;
}