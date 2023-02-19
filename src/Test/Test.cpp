#include <boost/process.hpp>
#include <boost/process/windows.hpp>

#include <iostream>

namespace bp = boost::process;

std::string exec(const char* cmd) {
	bp::ipstream m_read;
	bp::ipstream m_read_err;
	bp::child reader(cmd, bp::std_out > m_read, bp::std_err > m_read_err, bp::windows::hide);
	reader.wait();
	std::string line;
	std::string result;
	std::string err;
	while (std::getline(m_read, line))
		result += line + "\n";
	while (std::getline(m_read_err, line))
		err += line + "\n";
	std::cout << result << std::endl;
	std::cout << err << std::endl;
	return result;
}

int main()
{
	exec("C:/Users/eldeeb/Desktop/test.cmd");
}