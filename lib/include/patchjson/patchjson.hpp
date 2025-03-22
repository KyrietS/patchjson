#pragma once
#include <string>
#include <filesystem>

namespace patchjson
{
    void patchFile(const std::filesystem::path& path, const std::string& pathToValue, const std::string& newValue);
    std::string patchContent(std::string content, const std::string& pathToValue, const std::string& newValue);
}