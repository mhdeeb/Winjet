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

std::wstring to_utf8(std::string_view str)
{
	if (str.empty()) {
		return L"";
	}

	const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, &str.at(0), (int)str.size(), nullptr, 0);
	if (size_needed <= 0) {
		throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
	}

	std::wstring result(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str.at(0), (int)str.size(), &result.at(0), size_needed);
	return result;
}

std::string from_utf8(std::wstring_view str)
{
	if (str.empty()) {
		return "";
	}

	const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0) {
		throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
	}

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), &result.at(0), size_needed, nullptr, nullptr);
	return result;
}