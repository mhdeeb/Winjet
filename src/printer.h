//
// Created by Mohamed Hussien El-Deeb on 12/20/2022.
//

#pragma once

#include <iostream>
#include <format>

namespace printer {
	std::string _sep = " ";
	std::string _end = "\n";

	void print() { std::cout << _end; }

	template<typename T, typename ...Args>
	void print(const T& t, Args &&... args) {
		std::cout << t << _sep;
		print(args...);
	}

	template <class... _Types>
	void printv(std::string_view _Fmt, _Types&&... _Args) {
		std::cout << std::vformat(_Fmt, std::make_format_args(_Args...));
	}

	void sep(std::string_view separator) {
		_sep = separator;
	}

	void end(std::string_view ending) {
		_end = ending;
	}
}
