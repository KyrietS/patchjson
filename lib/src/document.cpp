#include "patchjson/document.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <sstream>
#include <iomanip>

namespace patchjson
{
    void Document::load(std::string source)
    {
        this->source = std::move(source);
        Lexer lexer{this->source};
        Parser parser{lexer.tokenize()};
        object = parser.parse();
    }

    const JsonObject& Document::json() const
    {
        return object;
    }

    void Document::replace(const JsonPath& path, int newValue)
    {
        const auto& value = object.find(path);
        if (not value.isNumber())
        {
            throw std::invalid_argument{"Value at the specified path is not a number"};
        }

        size_t offset = value.token.position;
        size_t length = value.token.lexeme.size();
        std::string_view originalSource = source;
        std::string_view before = originalSource.substr(0, offset);
        std::string_view after = originalSource.substr(offset + length);
        std::string replacement = std::to_string(newValue);

        std::stringstream ss;
        ss << before << replacement << after;
        source = ss.str();

        load(source);
    }

    bool Document::replacable(const JsonPath& path) const
    {
        try
        {
            const auto& value = object.find(path);
            return value.isNumber(); // Only numbers are replacable.
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    const std::string& Document::content() const
    {
        return source;
    }
}