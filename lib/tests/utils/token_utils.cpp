#include "token_utils.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace token_utils
{
    Matcher<patchjson::Token> matchesToken(
        Matcher<patchjson::TokenType> type,
        Matcher<std::string> lexeme,
        Matcher<patchjson::Literal> literal,
        Matcher<size_t> position)
    {
        return AllOf(
            Field("type", &patchjson::Token::type, type),
            Field("lexeme", &patchjson::Token::lexeme, lexeme),
            Field("literal", &patchjson::Token::literal, literal),
            Field("position", &patchjson::Token::position, position));
    }

    Matcher<patchjson::Token> matchesToken(
        patchjson::TokenType type,
        Matcher<std::string> lexeme,
        patchjson::Literal literal,
        Matcher<size_t> position)
    {
        return matchesToken(type, lexeme, Eq(literal), position);
    }

    Matcher<patchjson::Token> matchesTokenPosition(patchjson::TokenType type, Matcher<size_t> position)
    {
        return matchesToken(type, _, _, position);
    }
}