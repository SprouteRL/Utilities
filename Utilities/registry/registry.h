#include <windows.h>
#include <string>
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
    // Helper function to map C++ types to registry value types
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

    // Static templated function to write registry values
    template<typename T>
    static void WriteRegistryValue(const std::string& path, const std::string& valueName, const T& valueData) {
        static_assert(std::is_same_v<T, DWORD> || std::is_same_v<T, std::string>,
                      "Unsupported type. Specialize RegistryTypeMapper for this type.");

        // Determine the root key and subkey
        HKEY hKeyRoot;
        std::string subKey;

        if (path.rfind("HKEY_CLASSES_ROOT\\", 0) == 0) {
            hKeyRoot = HKEY_CLASSES_ROOT;
            subKey = path.substr(18);
        } else if (path.rfind("HKEY_CURRENT_USER\\", 0) == 0) {
            hKeyRoot = HKEY_CURRENT_USER;
            subKey = path.substr(18);
        } else if (path.rfind("HKEY_LOCAL_MACHINE\\", 0) == 0) {
            hKeyRoot = HKEY_LOCAL_MACHINE;
            subKey = path.substr(19);
        } else if (path.rfind("HKEY_USERS\\", 0) == 0) {
            hKeyRoot = HKEY_USERS;
            subKey = path.substr(11);
        } else {
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
        if constexpr (std::is_same_v<T, DWORD>) {
            result = RegSetValueExA(hKey, valueName.c_str(), 0, RegistryTypeMapper<DWORD>::Type,
                                    reinterpret_cast<const BYTE*>(&valueData), sizeof(DWORD));
        } else if constexpr (std::is_same_v<T, std::string>) {
            result = RegSetValueExA(hKey, valueName.c_str(), 0, RegistryTypeMapper<std::string>::Type,
                                    reinterpret_cast<const BYTE*>(valueData.c_str()), static_cast<DWORD>(valueData.size() + 1));
        }

        if (result != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            throw RegistryException("Error setting registry value.", result);
        }

        // Close the registry key
        RegCloseKey(hKey);
    }
};
