#pragma once
#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>
#include <shlobj.h>

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

	static std::string PrintBool(const bool& boolean);

	static bool SaveToJson(const nlohmann::json& jsonData, const std::string& filename);
	static nlohmann::json LoadFromJson(const std::string& filename);

	static std::string GetSpecialFolderPath(const std::string& folderName);
	static bool StartProgram(const std::string& exePath);
};