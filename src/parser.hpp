#pragma once
#include "json_object.hpp"
#include "lexer.hpp"
#include <stdexcept>
#include <span>


namespace json_manip
{
    struct ParsingError : std::runtime_error
    {
        using std::runtime_error::runtime_error;
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
