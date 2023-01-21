#include "ssql.h"

#include <sstream>
#include <array>
#include <sqlext.h>

ssql::ssql() {
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle);
	SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle);
}

bool ssql::connect(SQLCHAR* connString) {
	SQLDriverConnectA(sqlConnHandle, nullptr, connString, SQL_NTS, nullptr, 1024, nullptr, SQL_DRIVER_NOPROMPT);
	bool result = SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
	connected = true;
	return result == SQL_SUCCESS;
}

bool ssql::quary(const std::string& quary) {
	return SQLExecDirectA(sqlStmtHandle, (SQLCHAR*)quary.c_str(), SQL_NTS) == SQL_SUCCESS;
}

bool ssql::getResults(std::string& result, const std::string& delimiter) {
	std::stringstream ss;
	if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
		std::array<SQLCHAR, SQL_RESULT_LEN>data{};
		SQLUSMALLINT i = 1;
		if (SQLGetData(sqlStmtHandle, i, SQL_CHAR, &data, SQL_RESULT_LEN, nullptr) == SQL_SUCCESS) {
			ss << data.data();
		}
		while (SQLGetData(sqlStmtHandle, i + 1, SQL_CHAR, &data, SQL_RESULT_LEN, nullptr) == SQL_SUCCESS) {
			ss << delimiter << data.data();
			i++;
		}
		result = ss.str();
		return true;
	}
	return false;
}

bool ssql::isConnected() const {
	return connected;
}

ssql::~ssql() {
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}