#pragma once
#include <string>
#include <variant>
#include <optional>
#include <format>
#include <string_view>

namespace patchjson
{
    enum class TokenType
    {
        OpenBrace,
        CloseBrace,
        OpenBracket,
        CloseBracket,
        Colon,
        Comma,
        String,
        Number,
        True,
        False,
        Null,

        EndOfFile
    };



    using Lexeme = std::string_view;
    using Literal = std::optional<std::variant<std::string_view, double, bool>>;

    struct Token
    {
        TokenType type {};
        Lexeme lexeme {};
        Literal literal {};

        size_t position = 0;
    };

    std::string to_string(TokenType type);

    std::ostream& operator<<(std::ostream& os, const patchjson::TokenType& type);
    std::ostream& operator<<(std::ostream& os, const patchjson::Token& token);

}


template <>
struct std::formatter<patchjson::TokenType> : std::formatter<std::string>
{
    auto format(patchjson::TokenType type, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(to_string(type), ctx);
    }
};

