#include "token_utils.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

namespace token_utils
{
    Matcher<json_manip::Token> matchesToken(
        Matcher<json_manip::TokenType> type,
        Matcher<std::string> lexeme,
        Matcher<json_manip::Literal> literal,
        Matcher<size_t> position)
    {
        return AllOf(
            Field("type", &json_manip::Token::type, type),
            Field("lexeme", &json_manip::Token::lexeme, lexeme),
            Field("literal", &json_manip::Token::literal, literal),
            Field("position", &json_manip::Token::position, position));
    }

    Matcher<json_manip::Token> matchesToken(
        json_manip::TokenType type,
        Matcher<std::string> lexeme,
        json_manip::Literal literal,
        Matcher<size_t> position)
    {
        return matchesToken(type, lexeme, Eq(literal), position);
    }

    Matcher<json_manip::Token> matchesTokenPosition(json_manip::TokenType type, Matcher<size_t> position)
    {
        return matchesToken(type, _, _, position);
    }
}