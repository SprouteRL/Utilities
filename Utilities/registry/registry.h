#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <type_traits>

class RegistryException : public std::runtime_error {
public:
    RegistryException(const std::string& message, LONG errorCode)
        : std::runtime_error(message), errorCode(errorCode) {}

    LONG ErrorCode() const noexcept { return errorCode; }

private:
    LONG errorCode;
};

class Registry {
public:
    // Helper function to map registry value types to C++ types
    template<typename T>
    struct RegistryTypeMapper;

    template<>
    struct RegistryTypeMapper<DWORD> {
        static constexpr DWORD Type = REG_DWORD;
    };

    template<>
    struct RegistryTypeMapper<std::string> {
        static constexpr DWORD Type = REG_SZ;
    };

    // Static templated function to read registry values
    template<typename T>
    static T ReadRegistryValue(const std::string& path, const std::string& valueName) {
        static_assert(std::is_same_v<T, DWORD> || std::is_same_v<T, std::string>,
            "Unsupported type. Specialize RegistryTypeMapper for this type.");

        // Determine the root key and subkey
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
            throw RegistryException("Invalid registry root key.", ERROR_INVALID_PARAMETER);
        }

        // Open the specified registry key
        HKEY hKey;
        LONG result = RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_QUERY_VALUE, &hKey);
        if (result != ERROR_SUCCESS) {
            throw RegistryException("Error opening registry key.", result);
        }

        // Query the size and type of the registry value
        DWORD dataType = 0;
        DWORD dataSize = 0;
        result = RegQueryValueExA(hKey, valueName.c_str(), nullptr, &dataType, nullptr, &dataSize);
        if (result != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            throw RegistryException("Error querying registry value size.", result);
        }

        // Check if the data type matches the expected type
        if (dataType != RegistryTypeMapper<T>::Type) {
            RegCloseKey(hKey);
            throw RegistryException("Registry value type mismatch.", ERROR_INVALID_DATATYPE);
        }

        // Allocate buffer and retrieve the actual data
        T value;
        if constexpr (std::is_same_v<T, DWORD>) {
            if (dataSize != sizeof(DWORD)) {
                RegCloseKey(hKey);
                throw RegistryException("Unexpected DWORD size.", ERROR_INVALID_DATA);
            }
            result = RegQueryValueExA(hKey, valueName.c_str(), nullptr, nullptr,
                reinterpret_cast<LPBYTE>(&value), &dataSize);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::vector<char> buffer(dataSize);
            result = RegQueryValueExA(hKey, valueName.c_str(), nullptr, nullptr,
                reinterpret_cast<LPBYTE>(buffer.data()), &dataSize);
            value.assign(buffer.data(), dataSize - 1); // Exclude null terminator
        }

        // Close the registry key
        RegCloseKey(hKey);

        if (result != ERROR_SUCCESS) {
            throw RegistryException("Error reading registry value.", result);
        }

        return value;
    }

    // Static function to modify a DWORD registry value
    static void ModifyRegistryValue(const std::string& path, const std::string& valueName, DWORD valueData) {
        // Determine the root key and subkey
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
            throw RegistryException("Invalid registry root key.", ERROR_INVALID_PARAMETER);
        }

        // Open or create the specified registry key
        HKEY hKey;
        LONG result = RegCreateKeyExA(hKeyRoot, subKey.c_str(), 0, nullptr, 0,
            KEY_SET_VALUE, nullptr, &hKey, nullptr);
        if (result != ERROR_SUCCESS) {
            throw RegistryException("Error opening or creating registry key.", result);
        }

        // Set the registry value
        result = RegSetValueExA(hKey, valueName.c_str(), 0, REG_DWORD,
            reinterpret_cast<const BYTE*>(&valueData), sizeof(DWORD));
        if (result != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            throw RegistryException("Error setting registry value.", result);
        }

        // Close the registry key
        RegCloseKey(hKey);
    }
};
