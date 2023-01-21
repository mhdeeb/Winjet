#include "process.h"

#include <memory>

std::string exec(const char* cmd) {
	char buffer[128];
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), &_pclose);
	if (!pipe)
		throw "popen() failed!";
	while (fgets(buffer, 128, pipe.get()) != nullptr)
		result += buffer;
	return result;
}