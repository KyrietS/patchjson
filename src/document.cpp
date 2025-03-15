#include "document.hpp"
#include "lexer.hpp"
#include "parser.hpp"

namespace json_manip
{
    void Document::load(std::string_view source)
    {
        this->source = std::string{source};
        Lexer lexer{this->source};
        Parser parser{lexer.tokenize()};
        object = parser.parseObject();
    }

    const JsonObject& Document::json() const
    {
        return object;
    }

    const std::string& Document::content() const
    {
        return source;
    }
}