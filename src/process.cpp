#include "process.h"

#include <memory>
#include <boost/process.hpp>

std::string exec(const char* cmd) {
	namespace bp = boost::process;
	bp::ipstream m_read;
	bp::child reader(cmd, bp::std_out > m_read);
	reader.wait();
	std::string line;
	std::string result;
	while (std::getline(m_read, line))
		result += line + "\n";
	return result;
	//char buffer[128];
	//std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), &_pclose);
	//if (!pipe)
	//	throw "popen() failed!";
	//while (fgets(buffer, 128, pipe.get()) != nullptr)
	//	result += buffer;
	//return result;
}