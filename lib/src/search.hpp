# pragma once
#include <string_view>
#include <optional>
#include "token.hpp"
#include "json_path.hpp"
#include "lexer.hpp"


namespace patchjson
{
    struct SearchError : std::runtime_error
    {
        SearchError(const std::string& message, std::optional<Token> token) : std::runtime_error(message), token(token) {}
        std::optional<Token> token;
    };

    class Search
    {
    public:
        Search(std::string_view source);

        Token findValue(const JsonPath& path);

    private:
        Token searchObject(std::span<const std::string> path);
        Token searchArray(std::span<const std::string> path);
        Token searchValue(std::span<const std::string> path);
        Token getValue();

        void ignoreObject();
        void ignoreArray();
        void ignoreValue();


        Token consume(TokenType type);
        Token peek() const;
        bool match(TokenType type) const;

        Lexer lexer;
        Token nextToken {};
    };
}