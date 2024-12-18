#pragma once
#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>

#include "../dependencies/json.hpp"
#include "../logger/logger.h"

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

	static bool SaveToJson(const nlohmann::json& jsonData, const std::string& filename);
	static nlohmann::json LoadFromJson(const std::string& filename);
};