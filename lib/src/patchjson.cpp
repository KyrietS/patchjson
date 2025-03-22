#include "patchjson/patchjson.hpp"
#include <fstream>
#include <sstream>
#include "search.hpp"
#include "lexer.hpp"
#include "token.hpp"

namespace patchjson
{
namespace
{
    std::string replace(std::string_view source, const JsonPath& path, std::string_view replacement)
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

        std::stringstream ss;
        ss << before << replacement << after;
        return ss.str();
    }

    void validate(const std::string& value)
    {
        auto tokens = Lexer{value}.tokenize();
        if (tokens.size() != 2 || tokens.at(1).type != TokenType::EndOfFile)
        {
            throw std::invalid_argument{"Invalid replacement: " + value};
        }

        for (unsigned char c : value)
        {
            if (std::iscntrl(c))
            {
                throw std::invalid_argument{"Invalid character ({" + value + "}) in replacement: "};
            }
        }

        switch(tokens.at(0).type)
        {
            case TokenType::Number:
            case TokenType::String:
            case TokenType::True:
            case TokenType::False:
            case TokenType::Null:
                break;
            default:
                throw std::invalid_argument{"Invalid replacement: " + value};
        }
    }
}

    void patchFile(const std::filesystem::path& path, const std::string& pathToValue, const std::string& replacement)
    {
        std::ifstream input{path};
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string patchedContent = patchContent(buffer.str(), pathToValue, replacement);
        input.close();

        std::ofstream output{path, std::ios::trunc};
        output << patchedContent;
    }

    std::string patchContent(std::string content, const std::string& pathToValue, const std::string& replacement)
    {
        validate(replacement);
        JsonPath path{pathToValue};
        return replace(content, path, replacement);
    }
}