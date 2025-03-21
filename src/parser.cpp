#include "parser.hpp"
#include <memory>
#include <format>
#include <variant>
#include "token.hpp"

namespace patchjson
{
namespace
{
    void setLocationAndParent(JsonObject& object, JsonValue* parent)
    {
        for(auto& [key, value] : object)
        {
            value.location = key;
            value.parent = parent;
            if (value.isObjectOrArray())
            {
                setLocationAndParent(value.getObject(), &value);
            }
        }
    }
}

    JsonObject Parser::parse()
    {
        JsonObject object = parseObject();
        setLocationAndParent(object, nullptr);
        return object;
    }

    JsonObject Parser::parseObject()
    {
        JsonObject object;
        object.beginToken = consume(TokenType::OpenBrace);

        while (not match(TokenType::CloseBrace))
        {
            std::string_view key = std::get<std::string_view>(consume(TokenType::String).literal.value());
            consume(TokenType::Colon);
            JsonValue value = parseValue();
            object.add(std::string{key}, std::move(value));

            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
        }

        object.endToken = consume(TokenType::CloseBrace);

        return object;
    }

    JsonValue Parser::parseValue()
    {
        if (match(TokenType::OpenBrace))
        {
            Token token = peek();
            return {token, std::make_unique<JsonObject>(parseObject())};
        }
        else if (match(TokenType::OpenBracket))
        {
            Token token = peek();
            return {token, std::make_unique<JsonObject>(parseArray())};
        }
        else if (match(TokenType::String))
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
            throw ParsingError("Unexpected token");
        }
    }

    JsonObject Parser::parseArray()
    {
        JsonObject array;
        array.beginToken = consume(TokenType::OpenBracket);

        size_t index = 0;
        while (not match(TokenType::CloseBracket))
        {
            array.add(std::to_string(index), parseValue());
            index++;

            if (match(TokenType::Comma))
            {
                consume(TokenType::Comma);
            }
        }

        array.endToken = consume(TokenType::CloseBracket);
        return array;
    }

    bool Parser::match(TokenType type) const
    {
        return peek().type == type;
    }

    const Token& Parser::peek() const
    {
        if (nextToken >= tokens.size())
        {
            throw ParsingError("Unexpected end of input");
        }

        return tokens[nextToken];
    }

    const Token& Parser::consume(TokenType type)
    {
        const auto& token = peek();
        if (token.type != type)
        {
            throw ParsingError(std::format("Unexpected token at offset: {}. Expected: '{}', but got: '{}'", token.position, type, token.lexeme));
        }

        return tokens[nextToken++];
    }
}