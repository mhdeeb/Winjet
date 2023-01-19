#include "my_ssql.h"

std::string my_ssql::code_to_name_msg(int code) {
	auto connString = (SQLWCHAR*)L"DRIVER={SQL Server};Server=np:\\\\.\\pipe\\LOCALDB#86E7D6C9\\tsql\\query;database=Win32Const";

	ssql sql;

	sql.connect(connString);

	std::string result;

	if (sql.isConnected()) {
		std::string quary = std::string("SELECT name FROM Win32Msg WHERE code=") + std::to_string(code);
		sql.quary(quary);
		sql.getResults(result);
	}
	return result;
}

std::string my_ssql::code_to_name_vk(int code) {
	auto connString = (SQLWCHAR*)L"DRIVER={SQL Server};Server=np:\\\\.\\pipe\\LOCALDB#86E7D6C9\\tsql\\query;database=Win32Const";

	ssql sql;

	sql.connect(connString);

	std::string result;

	if (sql.isConnected()) {
		std::string quary = std::string("SELECT name FROM Win32Vk WHERE code=") + std::to_string(code);
		sql.quary(quary);
		sql.getResults(result);
	}
	return result;
}