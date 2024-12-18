#pragma once
#include <Windows.h>
#include <sstream>
#include <string>

class Utilities
{
public:
	template <typename ... Ty>
	static std::string Stringify(const Ty&... args)
	{
		std::stringstream oss;
		(oss << ... << args);
		return oss.str();
	}

	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& str);


};