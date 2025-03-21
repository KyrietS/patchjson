#include "search.hpp"
#include "lexer.hpp"
#include <span>
#include <stdexcept>

namespace patchjson
{
    Search::Search(std::string_view source)
        : lexer { source }
    {
        nextToken = lexer.nextToken();
    }

    Token Search::findValue(const JsonPath& path)
    {
        return searchObject(path);
    }

    Token Search::searchObject(std::span<const std::string> path)
    {
        Token beginObject = consume(TokenType::OpenBrace);
        if (path.empty())
        {
            return beginObject;
        }

        while (not match(TokenType::CloseBrace))
        {
            std::string_view key = std::get<std::string_view>(consume(TokenType::String).literal.value());
            consume(TokenType::Colon);
            if (key == path[0])
            {
                path = path.subspan(1);
                return searchValue(path);
            }
            else
            {
                ignoreValue();
            }

            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
        }
        consume(TokenType::CloseBrace);
        throw std::runtime_error(std::format("Key '{}' not found", path[0]));
    }

    Token Search::searchArray(std::span<const std::string> path)
    {
        Token beginArray = consume(TokenType::OpenBracket);
        if (path.empty())
        {
            return beginArray;
        }

        size_t index = 0;
        while (not match(TokenType::CloseBracket))
        {
            if (std::to_string(index) == path[0])
            {
                path = path.subspan(1);
                return searchValue(path);
            }
            else
            {
                ignoreValue();
            }

            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
            index++;
        }

        throw std::runtime_error(std::format("Index '{}' not found", path[0]));
    }

    Token Search::searchValue(std::span<const std::string> path)
    {
        if (match(TokenType::OpenBrace))
        {
            return searchObject(path);
        }
        else if (match(TokenType::OpenBracket))
        {
            return searchArray(path);
        }
        else if (path.empty())
        {
            return getValue();
        }
        else
        {
            throw std::runtime_error(std::format("Key '{}' not found", path[0]));
        }
    }

    Token Search::getValue()
    {
        if (match(TokenType::String))
        {
            return consume(TokenType::String);
        }
        else if (match(TokenType::Number))
        {
            return consume(TokenType::Number);
        }
        else if (match(TokenType::True))
        {
            return consume(TokenType::True);
        }
        else if (match(TokenType::False))
        {
            return consume(TokenType::False);
        }
        else if (match(TokenType::Null))
        {
            return consume(TokenType::Null);
        }
        else
        {
            throw std::runtime_error(std::format("Unexpected token: '{}'", nextToken.lexeme));
        }
    }

    void Search::ignoreObject()
    {
        consume(TokenType::OpenBrace);
        while (not match(TokenType::CloseBrace))
        {
            consume(TokenType::String);
            consume(TokenType::Colon);
            ignoreValue();
            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
        }
        consume(TokenType::CloseBrace);
    }

    void Search::ignoreArray()
    {
        consume(TokenType::OpenBracket);
        while (not match(TokenType::CloseBracket))
        {
            ignoreValue();
            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
        }
        consume(TokenType::CloseBracket);
    }

    void Search::ignoreValue()
    {
        if (match(TokenType::OpenBrace))
        {
            ignoreObject();
        }
        else if (match(TokenType::OpenBracket))
        {
            ignoreArray();
        }
        else
        {
            getValue();
        }
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