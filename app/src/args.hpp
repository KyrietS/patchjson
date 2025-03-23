#pragma once
#include <string>
#include <optional>

namespace patchjson_app
{
    struct Args
    {
        Args(int argc, char* argv[]);

        std::optional<std::string> file {};
        std::optional<std::string> path {};
        std::optional<std::string> replacement {};
        bool help {};
        bool version {};
    };
}