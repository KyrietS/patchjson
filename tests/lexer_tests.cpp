#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/token_utils.hpp"

#include "lexer.hpp"
#include <iostream>

using namespace testing;
using namespace patchjson;
using namespace token_utils;


TEST(LexerTests, OpenBrace)
{
    auto tokens = Lexer{"{"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::OpenBrace, "{", Eq(std::nullopt)));
}

TEST(LexerTests, CloseBrace)
{
    auto tokens = Lexer{"}"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::CloseBrace, "}", Eq(std::nullopt)));
}

TEST(LexerTests, OpenBracket)
{
    auto tokens = Lexer{"["}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::OpenBracket, "[", Eq(std::nullopt)));
}

TEST(LexerTests, CloseBracket)
{
    auto tokens = Lexer{"]"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::CloseBracket, "]", Eq(std::nullopt)));
}

TEST(LexerTests, Colon)
{
    auto tokens = Lexer{":"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Colon, ":", Eq(std::nullopt)));
}

TEST(LexerTests, Comma)
{
    auto tokens = Lexer{","}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Comma, ",", Eq(std::nullopt)));
}

TEST(LexerTests, String)
{
    auto tokens = Lexer{"\"foo\""}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::String, "\"foo\"", Optional(VariantWith<std::string>("foo"))));
}

TEST(LexerTests, True)
{
    auto tokens = Lexer{"true"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::True, "true", Optional(true)));
}

TEST(LexerTests, False)
{
    auto tokens = Lexer{"false"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::False, "false", Optional(false)));
}

TEST(LexerTests, Null)
{
    auto tokens = Lexer{"null"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Null, "null", Eq(std::nullopt)));
}

TEST(LexerTests, Number)
{
    auto tokens = Lexer{"42"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Number, "42", Optional(42.0)));
}

TEST(LexerTests, DecimalNumber)
{
    auto tokens = Lexer{"42.5"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Number, "42.5", Optional(42.5)));
}

TEST(LexerTests, NegativeNumber)
{
    auto tokens = Lexer{"-42"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Number, "-42", Optional(-42.0)));
}

TEST(LexerTests, NumberWithComma)
{
    auto tokens = Lexer{"42,"}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Number, "42", Optional(42.0)));
    ASSERT_THAT(tokens.at(1), matchesToken(TokenType::Comma, ","));
}

TEST(LexerTests, SimpleObject)
{
    std::string_view source = R"({ "foo": "bar" })";
    auto tokens = Lexer{source}.tokenize();

    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::OpenBrace, "{"));
    ASSERT_THAT(tokens.at(1), matchesToken(TokenType::String, "\"foo\"", Optional(std::string("foo"))));
    ASSERT_THAT(tokens.at(2), matchesToken(TokenType::Colon, ":"));
    ASSERT_THAT(tokens.at(3), matchesToken(TokenType::String, "\"bar\"", Optional(std::string("bar"))));
    ASSERT_THAT(tokens.at(4), matchesToken(TokenType::CloseBrace, "}"));

    ASSERT_THAT(tokens.at(5), matchesToken(TokenType::EndOfFile, ""));
}

TEST(LexerTests, NumberPosition)
{
    auto tokens = Lexer{" 123"}.tokenize();
    ASSERT_EQ(tokens.at(0).position, 1);
}

TEST(LexerTests, StringPosition)
{
    auto tokens = Lexer{"  \"foo\""}.tokenize();
    ASSERT_EQ(tokens.at(0).position, 2);
}

TEST(LexerTests, BracePosition)
{
    auto tokens = Lexer{"123{ "}.tokenize();
    ASSERT_EQ(tokens.at(1).position, 3);
}

TEST(LexerTests, Multiline)
{
    auto tokens = Lexer{"123\n-456.5\r\n[ \"foo\"]: \n"}.tokenize();

    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::Number, "123", Optional(123.0), 0));
    ASSERT_THAT(tokens.at(1), matchesToken(TokenType::Number, "-456.5", Optional(-456.5), 4));
    ASSERT_THAT(tokens.at(2), matchesToken(TokenType::OpenBracket, "[", Eq(std::nullopt), 12));
    ASSERT_THAT(tokens.at(3), matchesToken(TokenType::String, "\"foo\"", Optional(std::string{"foo"}), 14));
    ASSERT_THAT(tokens.at(4), matchesToken(TokenType::CloseBracket, "]", Eq(std::nullopt), 19));
    ASSERT_THAT(tokens.at(5), matchesToken(TokenType::Colon, ":", Eq(std::nullopt), 20));
    ASSERT_THAT(tokens.at(6), matchesToken(TokenType::EndOfFile, "", Eq(std::nullopt), 23));
}