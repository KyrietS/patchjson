#pragma once
#include <expected>
#include "token.hpp"
#include <vector>
#include <stdexcept>

namespace patchjson
{
    struct LexerError
    {
        std::string message;
        size_t line;
        size_t column;
    };

    class Lexer
    {
    public:
        Lexer(std::string_view source) : source(source) {}

        std::vector<Token> tokenize();
        Token nextToken();

    private:
        bool isAtEnd() const;
        void consumeWhitespaces();

        Token readToken(TokenType type, char expectedChar);
        Token readToken(TokenType type, std::string_view expectedLexeme, const patchjson::Literal& = {});
        Token readStringToken();
        Token readNumberToken();

        std::string_view consume(size_t length);

        size_t line = 1;
        size_t column = 1;
        size_t position = 0;
        std::string_view source {};
    };
}