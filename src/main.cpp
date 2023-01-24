#include "controller.h"

#define ENTRY main

int WINAPI ENTRY(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Controller controller(hInstance);

	//std::shared_ptr<WindowClass> base = std::make_shared<HiddenWindow>(
	//	hInstance,
	//	0, 0, 0, 0,
	//	L"Base");
	//controller.AddWindow(base);

	//std::shared_ptr<WindowClass> canvas = std::make_shared<CanvasWindow>(
	//	hInstance,
	//	0, 0, 0, 0,
	//	L"Canvas");
	//SetParent(canvas->GetHwnd(), base->GetHwnd());
	//controller.AddWindow(canvas);

	std::shared_ptr<WindowClass> widget = std::make_shared<WidgetWindow>(
		hInstance,
		0, 0, 400, 40,
		L"Widget");
	//SetParent(widget->GetHwnd(), canvas->GetHwnd());
	controller.AddWindow(widget);

	//controller.LoadWindows("save/Windows");
	int exitCode = controller.run();
	controller.SaveWindows("save/Windows");
	return exitCode;
}