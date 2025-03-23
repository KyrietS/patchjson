#include "token_utils.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace token_utils
{
    Matcher<patchjson::Token> matchesToken(
        Matcher<patchjson::TokenType> type,
        Matcher<std::string> lexeme,
        Matcher<patchjson::Literal> literal)
    {
        return AllOf(
            Field("type", &patchjson::Token::type, type),
            Field("lexeme", &patchjson::Token::lexeme, lexeme),
            Field("literal", &patchjson::Token::literal, literal));
    }

    Matcher<patchjson::Token> matchesToken(
        patchjson::TokenType type,
        Matcher<std::string> lexeme,
        patchjson::Literal literal)
    {
        return matchesToken(type, lexeme, Eq(literal));
    }

    Matcher<patchjson::Token> matchesTokenPosition(Matcher<size_t> position, Matcher<size_t> line, Matcher<size_t> column)
    {
        return AllOf(
            Field("type", &patchjson::Token::position, position),
            Field("line", &patchjson::Token::line, line),
            Field("column", &patchjson::Token::column, column));
    }
}