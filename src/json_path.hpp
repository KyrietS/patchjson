#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <cassert>
#include <span>

namespace patchjson
{
    class JsonPath
    {
    public:
        JsonPath(const std::vector<std::string>& path)  : path{path} {}
        JsonPath(std::string_view path)
        {
            assert(path.starts_with('/'));
            path.remove_prefix(1);

            while (not path.empty())
            {
                auto pos = path.find('/');
                if (pos == std::string_view::npos)
                {
                    this->path.push_back(std::string{path});
                    break;
                }

                this->path.push_back(std::string{path.substr(0, pos)});
                path.remove_prefix(pos + 1);
            }
        }
        JsonPath(const char* path) : JsonPath(std::string_view{path}) {}

        std::string toString() const
        {
            std::ostringstream oss;
            for (const auto& location : path)
            {
                oss << '/' << location;
            }
            return oss.str();
        }

        operator std::string() const { return toString(); }
        operator std::span<const std::string>() const { return path; }

        std::vector<std::string> path;
    };


}