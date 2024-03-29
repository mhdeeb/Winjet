#pragma once

#include <chrono>

#include "includes/json_fwd.hpp"
#include <wtypes.h>

std::string return_current_time_and_date();

class Serializable {
public:
	virtual nlohmann::json Serialize() const = 0;
	virtual ~Serializable() = default;
};

COLORREF HexToRGB(std::string_view hex);

std::string RGBToHex(COLORREF color);

std::wstring to_utf8(std::string_view str);
std::string from_utf8(std::wstring_view str);
