#include "patchjson/patchjson.hpp"
#include <fstream>
#include <sstream>
#include "search.hpp"

namespace patchjson
{
namespace
{
    std::string replace(std::string_view source, const JsonPath& path, int newValue)
    {
        Token valueToken = Search{source}.findValue(path);

        if (valueToken.type != TokenType::Number)
        {
            throw std::invalid_argument{"Value at the specified path is not a number"};
        }

        size_t offset = valueToken.position;
        size_t length = valueToken.lexeme.size();
        std::string_view originalSource = source;
        std::string_view before = originalSource.substr(0, offset);
        std::string_view after = originalSource.substr(offset + length);
        std::string replacement = std::to_string(newValue);

        std::stringstream ss;
        ss << before << replacement << after;
        return ss.str();
    }
}

    void patchFile(const std::filesystem::path& path, const std::string& pathToValue, const std::string& newValue)
    {
        std::ifstream input{path};
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string patchedContent = patchContent(buffer.str(), pathToValue, newValue);
        input.close();

        std::ofstream output{path, std::ios::trunc};
        output << patchedContent;
    }

    std::string patchContent(std::string content, const std::string& pathToValue, const std::string& newValue)
    {
        JsonPath path{pathToValue};
        int value = std::stoi(newValue);
        return replace(content, path, value);
    }
}