#pragma once
#include <string>
#include <Windows.h>
#include <optional>
#include <iostream>

class Registry
{
public:
    static void ModifyRegistryValue(const std::string& path, const std::string& valueName, DWORD valueData);
    static DWORD ReadRegistryValue(const std::string& path, const std::string& valueName);
};