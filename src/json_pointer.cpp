#include "json_pointer.hpp"
#include <sstream>

namespace json_manip
{
    JsonPath::JsonPath(const JsonPointer& pointer)
        : tokens{pointer.tokens}
    {
    }

    std::string JsonPointer::to_string() const
    {
        std::ostringstream oss;
        for (const auto& token : tokens)
        {
            oss << '/' << token;
        }
        return oss.str();
    }

    JsonPath JsonPointer::to_path() const
    {
        return JsonPath{*this};
    }
}