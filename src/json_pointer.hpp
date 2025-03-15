#pragma once
#include <variant>
#include <string>
#include <vector>
#include <span>
#include <cassert>

namespace json_manip
{
    class JsonPointer;

    class JsonPath
    {
    public:
        JsonPath(const JsonPointer& pointer);

        const std::string& pop()
        {
            assert(not tokens.empty());
            const auto& token = tokens.front();
            tokens = tokens.subspan(1);
            return token;
        }

        std::span<const std::string> tokens;
    };

    class JsonPointer
    {
    public:
        JsonPointer(const std::vector<std::string>& tokens) : tokens{tokens} {}
        std::string to_string() const;
        JsonPath to_path() const;

        std::vector<std::string> tokens;
    };


}