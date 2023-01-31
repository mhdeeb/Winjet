#include "util.h"

std::string return_current_time_and_date() {
	return std::format("{:%Y-%m-%d %X}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
}

COLORREF HexToRGB(std::string_view hex) {
	if (hex[0] == '#') hex = hex.substr(1);
	if (hex.size() != 6) return RGB(0, 0, 0);
	int r = std::stoi(std::string(hex.substr(0, 2)), nullptr, 16);
	int g = std::stoi(std::string(hex.substr(2, 2)), nullptr, 16);
	int b = std::stoi(std::string(hex.substr(4, 2)), nullptr, 16);
	return RGB(r, g, b);
}

std::string RGBToHex(COLORREF color) {
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	return std::format("#{:02X}{:02X}{:02X}", r, g, b);
}