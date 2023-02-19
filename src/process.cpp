#include "process.h"

#include <memory>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>

namespace bp = boost::process;

std::string exec(const char* cmd) {
	bp::ipstream read_output;
	bp::ipstream read_error;
	bp::child reader(cmd, bp::std_out > read_output, bp::std_err > read_error, bp::windows::hide);
	reader.wait();
	std::string line;
	std::string result;
	std::string error;
	while (std::getline(read_output, line))
		result += line + "\n";
	while (std::getline(read_error, line))
		error += line + "\n";
	return result + error;
}

std::string exec(std::string cmd) {
	return exec(cmd.c_str());
}