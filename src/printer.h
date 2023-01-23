//
// Created by Mohamed Hussien El-Deeb on 12/20/2022.
//

#pragma once

#include <iostream>
#include <format>

namespace printer {
	std::string _sep = " ";
	std::string _end = "\n";

	std::wstring _wsep = L" ";
	std::wstring _wend = L"\n";

	void print() { std::cout << _end; }
	void wprint() { std::wcout << _wend; }

	template<typename T, typename ...Args>
	void print(const T& t, Args &&... args) {
		std::cout << t << _sep;
		print(args...);
	}

	template<typename T, typename ...Args>
	void wprint(const T& t, Args &&... args) {
		std::wcout << t << _wsep;
		wprint(args...);
	}

	template <class... _Types>
	void printv(std::string_view _Fmt, _Types&&... _Args) {
		std::cout << std::vformat(_Fmt, std::make_format_args(_Args...));
	}

	template <class... _Types>
	void wprintv(std::string_view _Fmt, _Types&&... _Args) {
		std::wcout << std::vformat(_Fmt, std::make_wformat_args(_Args...));
	}

	void sep(std::string_view separator) {
		_sep = separator;
	}

	void end(std::string_view ending) {
		_end = ending;
	}

	void wsep(std::wstring_view separator) {
		_wsep = separator;
	}

	void wend(std::wstring_view ending) {
		_wend = ending;
	}
}
