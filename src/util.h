#pragma once

#include <chrono>

std::string return_current_time_and_date() {
	auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	return std::format("{:%Y-%m-%d %X}", now);
}