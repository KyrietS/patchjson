#pragma once
#include "token.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace token_utils
{
namespace
{
    const json_manip::Token stringToken{json_manip::TokenType::String, "\"foo\"", "foo", 0};
    const json_manip::Token numberToken{json_manip::TokenType::Number, "0.5", 0.5, 0};
    const json_manip::Token trueToken{json_manip::TokenType::True, "true", true, 0};
    const json_manip::Token falseToken{json_manip::TokenType::False, "false", false, 0};
    const json_manip::Token nullToken{json_manip::TokenType::Null, "null", {}, 0};
    const json_manip::Token openBraceToken{json_manip::TokenType::OpenBrace, "{", {}, 0};
    const json_manip::Token openBracketToken{json_manip::TokenType::OpenBracket, "[", {}, 0};
}

    testing::Matcher<json_manip::Token> matchesToken(
        testing::Matcher<json_manip::TokenType> type,
        testing::Matcher<std::string> lexeme = testing::_,
        testing::Matcher<json_manip::Literal> literal = testing::_,
        testing::Matcher<size_t> position = testing::_);

    testing::Matcher<json_manip::Token> matchesTokenPosition(json_manip::TokenType type, testing::Matcher<size_t> position);
}