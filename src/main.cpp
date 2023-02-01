#include "controller.h"

#define ENTRY WinMain

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	if (FindWindow(L"CanvasWindow", L"Winjet")) {
		MessageBox(nullptr, L"Previous instance alredy running!", L"Warning", MB_OK);
		return 0;
	}
	Controller controller(hInstance);
	controller.LoadData("resource/save/data.json");
	int exitCode = controller.run();
	controller.SaveData("resource/save/data.json");
	return exitCode;
}