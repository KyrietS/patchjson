#pragma once
#include "token.hpp"
#include "json_path.hpp"
#include <string_view>

namespace patchjson
{
    Token findValue(std::string_view source, const JsonPath& path);
}