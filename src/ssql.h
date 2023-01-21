#pragma once

#include <string>

#include <Windows.h>
#include <sql.h>

constexpr int SQL_RESULT_LEN = 1024;

class ssql
{
private:
	SQLHANDLE sqlConnHandle = nullptr;
	SQLHANDLE sqlStmtHandle = nullptr;
	SQLHANDLE sqlEnvHandle = nullptr;
	bool connected = false;
public:
	ssql();

	bool connect(SQLCHAR* connString);

	bool quary(const std::string& quary);

	bool getResults(std::string& result, const std::string& delimiter = " ");

	bool isConnected() const;

	~ssql();
};
