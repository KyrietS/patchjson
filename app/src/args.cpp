#include "args.hpp"
#include <vector>
#include <string_view>

namespace patchjson_app
{
    namespace
    {
        std::optional<std::string> tryGet(std::vector<std::string_view>& args, size_t index)
        {
            if (index < args.size())
            {
                return std::string { args[ index ] };
            }
            return std::nullopt;
        }
    }

    Args::Args(int argc, char* argv[])
    {
        std::vector<std::string_view> args(argv, argv + argc);
        std::vector<std::string_view> positionals;

        for (std::string_view arg : args)
        {
            if (arg == "-h" or arg == "--help")
            {
                help = true;
            }
            else
            {
                positionals.push_back(arg);
            }
        }

        file = tryGet(positionals, 1);
        path = tryGet(positionals, 2);
        replacement = tryGet(positionals, 3);
    }
}