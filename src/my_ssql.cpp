#include "my_ssql.h"
#include "ssql.h"
#include "process.h"

#include <format>
#include <iostream>

std::string getConnectionString() {
	exec("SqlLocalDB s ProjectModels");
	std::string server = exec("SqlLocalDB i ProjectModels | grep -E \"([A-F]|[0-9]){8}\" -o");
	return std::vformat("DRIVER={{SQL Server}};Server=np:\\\\.\\pipe\\LOCALDB#{}\\tsql\\query;database=Win32Const", make_format_args(server.substr(0, server.size() - 1)));
}

const std::string connString = getConnectionString();

std::string my_ssql::code_to_name_msg(int code) {
	ssql sql;

	sql.connect((SQLCHAR*)connString.c_str());

	std::string result;

	if (sql.isConnected()) {
		sql.quary(std::format("SELECT name FROM win32wm where code={};", code));
		sql.getResults(result);
	}

	return result;
}

std::string my_ssql::code_to_name_vk(int code) {
	ssql sql;

	sql.connect((SQLCHAR*)connString.c_str());

	std::string result;

	if (sql.isConnected()) {
		sql.quary(std::format("SELECT name FROM win32vk where code={};", code));
		sql.getResults(result);
	}

	return result;
}