#pragma once
#include "json_object.hpp"
#include "token.hpp"
#include <stdexcept>
#include <span>


namespace patchjson
{
    struct ParserError : std::runtime_error
    {
        ParserError(const std::string& message, std::optional<Token> token = std::nullopt) : std::runtime_error(message), token(token) {}
        std::optional<Token> token;
    };

    class Parser
    {
    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

        JsonObject parse();

    private:
        JsonObject parseObject();
        JsonValue parseValue();
        JsonObject parseArray();

        bool match(TokenType type) const;
        const Token& peek() const;
        const Token& consume(TokenType);

        std::vector<Token> tokens;
        size_t nextToken = 0;
    };
}
