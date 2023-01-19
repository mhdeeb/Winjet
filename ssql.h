#pragma once
#include <sstream>
#include <Windows.h>
#include <sqlext.h>
#include <array>

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

	bool connect(SQLWCHAR* connString);

	bool quary(const std::string& quary);

	bool getResults(std::string& result, const std::string& delimiter = " ");

	bool isConnected() const;

	~ssql();
};
