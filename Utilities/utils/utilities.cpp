#include "utilities.h"

std::wstring Utilities::StringToWString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
	return wstr;
}

std::string Utilities::WStringToString(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
	return str;
}

std::string Utilities::PrintBool(const bool& boolean)
{
	return (boolean ? "True" : "False");
}

bool Utilities::SaveToJson(const nlohmann::json& jsonData, const std::string& filename)
{
	std::ofstream file(filename);
	if (file.is_open()) {
		file << jsonData.dump(4);
		file.close();
		return true;
	}
	return false;
}

nlohmann::json Utilities::LoadFromJson(const std::string& filename)
{
	std::ifstream file(filename);
	if (file.is_open()) {
		nlohmann::json jsonData;
		file >> jsonData;
		file.close();
		return jsonData;
	}
	else
	{
		Logger::Error("Failed loading ", filename, " from json.");
	}
}

std::string Utilities::GetSpecialFolderPath(const std::string& folderName)
{
	char path[MAX_PATH];

	if (folderName == "LOCALAPPDATA") {
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
			return std::string(path);
		}
	}
	else if (folderName == "APPDATA") {
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			return std::string(path);
		}
	}
	else if (folderName == "PROGRAMFILES") {
		if (GetEnvironmentVariableA("ProgramFiles", path, MAX_PATH)) {
			return std::string(path);
		}
	}
	else if (folderName == "PROGRAMFILESX86") {
		if (GetEnvironmentVariableA("ProgramFiles(x86)", path, MAX_PATH)) {
			return std::string(path);
		}
	}

	return ""; // Return empty string if folderName is not recognized or fails
}
