#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/token_utils.hpp"

#include "parser.hpp"

using namespace testing;
using namespace json_manip;
using namespace token_utils;

struct ParserTests : public Test
{
    JsonObject parseObject(std::string_view source)
    {
        auto tokens = Lexer{source}.tokenize();
        auto parser = Parser{tokens};
        return parser.parseObject();
    }

    void expectParsingError(std::string_view source)
    {
        auto tokens = Lexer{source}.tokenize();
        auto parser = Parser{tokens};
        EXPECT_THROW(parser.parseObject(), ParsingError);
    }
};

TEST_F(ParserTests, EmptyObject)
{
    JsonObject object = parseObject("{}");

    EXPECT_THAT(object, IsEmpty());
    EXPECT_THAT(object.beginToken, matchesToken(TokenType::OpenBrace, _, _, 0));
    EXPECT_THAT(object.endToken, matchesToken(TokenType::CloseBrace, _, _, 1));
}

TEST_F(ParserTests, InvalidObject)
{
    EXPECT_THROW(parseObject("{"), ParsingError);
}

TEST_F(ParserTests, ObjectWithOneMember)
{
    auto object = parseObject(R"({ "foo": 123 })");

    EXPECT_THAT(object.beginToken, matchesTokenPosition(TokenType::OpenBrace, 0));
    EXPECT_THAT(object.endToken, matchesTokenPosition(TokenType::CloseBrace, 13));
    EXPECT_THAT(object, SizeIs(1));

    EXPECT_EQ(object.at("foo"), 123.0);
}

TEST_F(ParserTests, ObjectWithTwoMembers)
{
    auto object = parseObject(R"({ "foo": 123, "bar": false })");

    EXPECT_THAT(object, SizeIs(2));
    EXPECT_EQ(object.at("foo"), 123.0);
    EXPECT_EQ(object.at("bar"), false);

    EXPECT_THAT(object.at("foo").token, matchesToken(TokenType::Number, "123", Optional(123.0), 9));
    EXPECT_THAT(object.at("bar").token, matchesToken(TokenType::False, "false", Optional(false), 21));
}

TEST_F(ParserTests, ObjectWithArray)
{
    auto object = parseObject(R"({ "foo": [1, {}, "bar"] })");

    EXPECT_THAT(object, SizeIs(1));
    const json_manip::JsonValue& value = object.at(0);

    ASSERT_TRUE(value.isArray());
    const JsonObject& array = value.getObject();

    EXPECT_THAT(array, SizeIs(3));
    EXPECT_EQ(array.at(0), 1.0);
    EXPECT_TRUE(array.at(1).isObject());
    EXPECT_EQ(array.at(2), "bar");
}
