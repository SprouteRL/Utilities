#pragma once
#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>
#include <shlobj.h>
#include <filesystem>

namespace fs = std::filesystem;

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

	static fs::path CreateFolder(const std::string& path, const std::string& folderName);
	static fs::path m_CreateFile(const std::string& directoryPath, const std::string& fileName, const std::string& content);

	static std::string ReadFileContent(const std::string& filePath);
	static bool WriteFileContent(const std::string& filePath, const std::string& content);
	static bool FileOrFolderExists(const std::string& path);
	static std::vector<std::string> GetSubFolders(const std::string& directoryPath);

	static bool DeleteFileOrFolder(const std::string& path);
};