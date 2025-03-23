#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/token_utils.hpp"

#include "lexer.hpp"
#include <iostream>

using namespace testing;
using namespace patchjson;
using namespace token_utils;

TEST(LexerTests, EmptySource)
{
    auto tokens = Lexer{""}.tokenize();
    ASSERT_THAT(tokens, SizeIs(1));
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::EndOfFile, ""));
}

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

MATCHER_P(StringViewEquals, expected, "Matches std::string_view") {
    return std::string(arg) == std::string(expected);
}

struct Foo {
    bool operator==(const Foo&) const { return true; }
};

TEST(LexerTests, String)
{
    auto tokens = Lexer{"\"foo\""}.tokenize();
    ASSERT_THAT(tokens.at(0), matchesToken(TokenType::String, "\"foo\"", Optional(patchjson::Literal{"foo"})));
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
    ASSERT_THAT(tokens.at(1), matchesToken(TokenType::String, "\"foo\"", Optional(std::string_view("foo"))));
    ASSERT_THAT(tokens.at(2), matchesToken(TokenType::Colon, ":"));
    ASSERT_THAT(tokens.at(3), matchesToken(TokenType::String, "\"bar\"", Optional(std::string_view("bar"))));
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

    ASSERT_THAT(tokens.at(0), AllOf(matchesToken(TokenType::Number, "123", Optional(123.0)), matchesTokenPosition(0, 1, 1)));
    ASSERT_THAT(tokens.at(1), AllOf(matchesToken(TokenType::Number, "-456.5", Optional(-456.5)), matchesTokenPosition(4, 2, 1)));
    ASSERT_THAT(tokens.at(2), AllOf(matchesToken(TokenType::OpenBracket, "[", Eq(std::nullopt)), matchesTokenPosition(12, 3, 1)));
    ASSERT_THAT(tokens.at(3), AllOf(matchesToken(TokenType::String, "\"foo\"", Optional(std::string_view{"foo"})), matchesTokenPosition(14, 3, 3)));
    ASSERT_THAT(tokens.at(4), AllOf(matchesToken(TokenType::CloseBracket, "]", Eq(std::nullopt)), matchesTokenPosition(19, 3, 8)));
    ASSERT_THAT(tokens.at(5), AllOf(matchesToken(TokenType::Colon, ":", Eq(std::nullopt)), matchesTokenPosition(20, 3, 9)));
    ASSERT_THAT(tokens.at(6), AllOf(matchesToken(TokenType::EndOfFile, "", Eq(std::nullopt)), matchesTokenPosition(23, 4, 1)));
}

struct LexerErrorTests : Test
{
    Matcher<LexerError> matchError(Matcher<std::string> message, Matcher<size_t> line, Matcher<size_t> column)
    {
        return AllOf(
            Field("message", &LexerError::message, message),
            Field("line", &LexerError::line, line),
            Field("column", &LexerError::column, column)
        );
    }

    void expectError(const std::string& source, const Matcher<LexerError>& matcher)
    {
        try
        {
            Lexer{source}.tokenize();
            FAIL() << "Expected LexerError";
        }
        catch(const LexerError& error)
        {
            EXPECT_THAT(error, matcher);
        }
        catch (...)
        {
            FAIL() << "Expected LexerError";
        }
    }
};

TEST_F(LexerErrorTests, UnexpectedCharacter)
{
    expectError("@", matchError(StartsWith("Unexpected character"), 1, 1));
}

TEST_F(LexerErrorTests, UnexpectedEndOfInput)
{
    expectError("tru", matchError(StartsWith("Unexpected end of input"), 1, 1));
}

TEST_F(LexerErrorTests, UnexpectedEndOfInput_String)
{
    expectError("\"", matchError(StartsWith("Unexpected end of input"), 1, 1));
}

TEST_F(LexerErrorTests, UnexpectedControlCharacter)
{
    expectError("\"foo\nbar\"", matchError(StartsWith("Unexpected control character"), 1, 1));
}

TEST_F(LexerErrorTests, UnexpectedToken)
{
    expectError("trux", matchError(StartsWith("Unexpected token"), 1, 1));
}

TEST_F(LexerErrorTests, InvalidNumber)
{
    expectError("--5", matchError(StartsWith("Invalid number"), 1, 1));
}
