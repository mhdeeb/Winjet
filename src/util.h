#pragma once

#include <chrono>

#include "includes/json_fwd.hpp"
#include <wtypes.h>

std::wstring return_current_time_and_date();

class Serializable {
public:
	virtual nlohmann::json Serialize() const = 0;
	virtual ~Serializable() = default;
};

COLORREF HexToRGB(std::wstring hex);

std::wstring RGBToHex(COLORREF color);