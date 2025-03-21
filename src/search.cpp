#include "search.hpp"
#include "lexer.hpp"
#include <span>
#include <stdexcept>

namespace patchjson
{
namespace
{
    Token consume(Lexer& lexer, TokenType type)
    {
        Token token = lexer.nextToken();
        if (token.type != type)
        {
            throw std::runtime_error("Unexpected token");
        }
        return token;
    }

    Token searchObject(Lexer& lexer, std::span<const std::string> path)
    {
        consume(lexer, TokenType::OpenBrace);
    }

}

    Search::Search(std::string_view source)
    : lexer{source}
    {
        nextToken = lexer.nextToken();
    }

    Token Search::findValue(const JsonPath& path)
    {


        // return object.find(path).token;
    }

    Token Search::consume(TokenType type)
    {
        Token token = nextToken;
        if (token.type != type)
        {
            throw std::runtime_error("Unexpected token");
        }
        nextToken = lexer.nextToken();
        return token;
    }

    Token Search::peek() const
    {
        return nextToken;
    }

    bool Search::match(TokenType type) const
    {
        return nextToken.type == type;
    }
}