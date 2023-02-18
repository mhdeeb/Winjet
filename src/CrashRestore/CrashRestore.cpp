#include <Windows.h>
#include <fstream>

int main() {
	std::ifstream file("resource/state.txt");

	bool isAutoHide;
	file >> isAutoHide;

	file.close();

	APPBARDATA abd = { sizeof abd };
	abd.lParam = isAutoHide ? ABS_AUTOHIDE : ABS_ALWAYSONTOP;
	SHAppBarMessage(ABM_SETSTATE, &abd);
	SHAppBarMessage(ABM_WINDOWPOSCHANGED, &abd);
}