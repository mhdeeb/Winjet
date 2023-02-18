#include <boost/process.hpp>

#include <iostream>

int main()
{
	namespace bp = boost::process;
	bp::ipstream m_read;
	bp::child reader("SqlLocalDB i ProjectModels", bp::std_out > m_read);
	reader.wait();
	std::string line;
	std::string result;
	while (std::getline(m_read, line))
		result += line + "\n";
	std::cout << result << std::endl;
	return 0;
}