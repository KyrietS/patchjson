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
        JsonPath() = default;
        JsonPath(const std::vector<std::string>& path)  : path{path} {}
        JsonPath(std::string_view path)
        {
            if (not path.starts_with('/')) {
                return;
            }

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

        void append(const std::string& segment)
        {
            path.push_back(segment);
        }

        JsonPath operator+(const std::string& segment) const
        {
            JsonPath newPath = *this;
            newPath.append(segment);
            return newPath;
        }

        bool operator==(const JsonPath& other) const
        {
            return path == other.path;
        }

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