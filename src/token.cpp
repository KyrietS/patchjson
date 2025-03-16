#include "token.hpp"
#include <ostream>

namespace patchjson
{

    std::string to_string(TokenType type)
    {
        switch (type)
        {
        case TokenType::OpenBrace: return "OpenBrace";
        case TokenType::CloseBrace: return "CloseBrace";
        case TokenType::String: return "String";
        case TokenType::Colon: return "Colon";
        case TokenType::OpenBracket: return "OpenBracket";
        case TokenType::CloseBracket: return "CloseBracket";
        case TokenType::Comma: return "Comma";
        case TokenType::Number: return "Number";
        case TokenType::True: return "True";
        case TokenType::False: return "False";
        case TokenType::Null: return "Null";
        default: return "Unknown";
        }
    }

std::ostream& operator<<(std::ostream& os, const patchjson::TokenType& type)
{
    return os << to_string(type);
}

std::ostream& operator<<(std::ostream& os, const patchjson::Token& token)
{
    os << "Token(type='" << token.type << "', lexeme='" << token.lexeme << "', literal='";
    if (token.literal.has_value())
    {
        std::visit([&os](const auto& value) { os << value; }, token.literal.value());
    }
    else
    {
        os << "nullopt";
    }

    os << "', position=" << token.position << ")";

    return os;
}
}