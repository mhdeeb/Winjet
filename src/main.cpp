#include "controller.h"
#include <fstream>

void init()
{
	if (FindWindow(L"CanvasWindow", L"Winjet")) {
		MessageBox(nullptr, L"Previous instance alredy running!", L"Warning", MB_OK);
		exit(0);
	}

	std::ofstream file("resource/state.txt");

	APPBARDATA abd = { sizeof abd };
	SHAppBarMessage(ABM_GETSTATE, &abd);
	file << (abd.lParam & ABS_AUTOHIDE) << ' ';

	file.close();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	init();
	Controller controller(hInstance);
	controller.LoadData("resource/save/data.json");
	int exitCode = controller.run();
	controller.SaveData("resource/save/data.json");
	return exitCode;
}

int main()
{
	init();
	Controller controller(GetModuleHandle(nullptr));
	controller.LoadData("resource/save/data.json");
	int exitCode = controller.run();
	controller.SaveData("resource/save/data.json");
	return exitCode;
}
