#pragma once
#include "token.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace token_utils
{
namespace
{
    const patchjson::Token stringToken{patchjson::TokenType::String, "\"foo\"", "foo", 0};
    const patchjson::Token numberToken{patchjson::TokenType::Number, "0.5", 0.5, 0};
    const patchjson::Token trueToken{patchjson::TokenType::True, "true", true, 0};
    const patchjson::Token falseToken{patchjson::TokenType::False, "false", false, 0};
    const patchjson::Token nullToken{patchjson::TokenType::Null, "null", {}, 0};
    const patchjson::Token openBraceToken{patchjson::TokenType::OpenBrace, "{", {}, 0};
    const patchjson::Token openBracketToken{patchjson::TokenType::OpenBracket, "[", {}, 0};
}

    testing::Matcher<patchjson::Token> matchesToken(
        testing::Matcher<patchjson::TokenType> type,
        testing::Matcher<std::string> lexeme = testing::_,
        testing::Matcher<patchjson::Literal> literal = testing::_);

    testing::Matcher<patchjson::Token> matchesTokenPosition(
        testing::Matcher<size_t> position,
        testing::Matcher<size_t> line,
        testing::Matcher<size_t> column);
}