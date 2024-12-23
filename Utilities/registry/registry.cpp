#include "registry.h"

void Registry::ModifyRegistryValue(const std::string& path, const std::string& valueName, DWORD valueData)
{
    // Split the root key and subkey
    HKEY hKeyRoot;
    std::string subKey;

    if (path.rfind("HKEY_CLASSES_ROOT\\", 0) == 0) {
        hKeyRoot = HKEY_CLASSES_ROOT;
        subKey = path.substr(18);
    }
    else if (path.rfind("HKEY_CURRENT_USER\\", 0) == 0) {
        hKeyRoot = HKEY_CURRENT_USER;
        subKey = path.substr(18);
    }
    else if (path.rfind("HKEY_LOCAL_MACHINE\\", 0) == 0) {
        hKeyRoot = HKEY_LOCAL_MACHINE;
        subKey = path.substr(19);
    }
    else if (path.rfind("HKEY_USERS\\", 0) == 0) {
        hKeyRoot = HKEY_USERS;
        subKey = path.substr(11);
    }
    else {
        std::cerr << "Invalid registry root key." << std::endl;
        return;
    }

    // Open the specified registry key
    HKEY hKey;
    LONG result = RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS) {
        std::cerr << "Error opening registry key: " << result << std::endl;
        return;
    }

    // Set the registry value
    result = RegSetValueExA(hKey, valueName.c_str(), 0, REG_DWORD, (const BYTE*)&valueData, sizeof(valueData));
    if (result != ERROR_SUCCESS) {
        std::cerr << "Error setting registry value: " << result << std::endl;
    }
    else {
        std::cout << "Registry value modified successfully." << std::endl;
    }

    // Close the registry key
    RegCloseKey(hKey);
}

DWORD Registry::ReadRegistryValue(const std::string& path, const std::string& valueName)
{
    // Split the root key and subkey
    HKEY hKeyRoot;
    std::string subKey;

    if (path.rfind("HKEY_CLASSES_ROOT\\", 0) == 0) {
        hKeyRoot = HKEY_CLASSES_ROOT;
        subKey = path.substr(18);
    }
    else if (path.rfind("HKEY_CURRENT_USER\\", 0) == 0) {
        hKeyRoot = HKEY_CURRENT_USER;
        subKey = path.substr(18);
    }
    else if (path.rfind("HKEY_LOCAL_MACHINE\\", 0) == 0) {
        hKeyRoot = HKEY_LOCAL_MACHINE;
        subKey = path.substr(19);
    }
    else if (path.rfind("HKEY_USERS\\", 0) == 0) {
        hKeyRoot = HKEY_USERS;
        subKey = path.substr(11);
    }
    else {
        std::cerr << "Invalid registry root key." << std::endl;
        return 0;
    }

    // Open the specified registry key
    HKEY hKey;
    LONG result = RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_QUERY_VALUE, &hKey);
    if (result != ERROR_SUCCESS) {
        std::cerr << "Error opening registry key: " << result << std::endl;
        return 0;
    }

    // Query the registry value
    DWORD dataType = 0;
    DWORD valueData = 0;
    DWORD dataSize = sizeof(DWORD);
    result = RegQueryValueExA(hKey, valueName.c_str(), nullptr, &dataType, reinterpret_cast<LPBYTE>(&valueData), &dataSize);
    if (result != ERROR_SUCCESS) {
        std::cerr << "Error reading registry value: " << result << std::endl;
        RegCloseKey(hKey);
        return 0;
    }

    // Check if the data type is DWORD
    if (dataType != REG_DWORD) {
        std::cerr << "The registry value is not a DWORD." << std::endl;
        RegCloseKey(hKey);
        return 0;
    }

    // Close the registry key
    RegCloseKey(hKey);

    return valueData;
}
