#pragma once

#include "ssql.h"

struct my_ssql {
	static std::string code_to_name_msg(int code);
	static std::string code_to_name_vk(int code);
};
