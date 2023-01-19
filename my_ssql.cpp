#include "my_ssql.h"
#include "process.h"

#include <format>
#include <iostream>

std::string getConnectionString() {
	exec("SqlLocalDB s ProjectModels");
	std::string server = exec("SqlLocalDB i ProjectModels | grep -E \"([A-F]|[0-9]){8}\" -o");
	return std::vformat("DRIVER={{SQL Server}};Server=np:\\\\.\\pipe\\LOCALDB#{}\\tsql\\query;database=Win32Const", make_format_args(server.substr(0, server.size() - 1)));
}

std::string connString = getConnectionString();

std::string my_ssql::code_to_name_msg(int code) {
	ssql sql;

	sql.connect((SQLCHAR*)connString.c_str());

	std::string result;

	if (sql.isConnected()) {
		std::string quary = std::string("SELECT name FROM Win32Msg WHERE code=") + std::to_string(code);
		sql.quary(quary);
		sql.getResults(result);
	}
	return result;
}

std::string my_ssql::code_to_name_vk(int code) {
	ssql sql;

	sql.connect((SQLCHAR*)connString.c_str());

	std::string result;

	if (sql.isConnected()) {
		std::string quary = std::string("SELECT name FROM Win32Vk WHERE code=") + std::to_string(code);
		sql.quary(quary);
		sql.getResults(result);
	}
	return result;
}