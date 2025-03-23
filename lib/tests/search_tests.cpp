#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "search.hpp"
#include "utils/token_utils.hpp"

using namespace testing;
using namespace patchjson;
using namespace token_utils;

struct SearchTests : Test
{
    Token findValue(std::string_view source, const char* path)
    {
        Search search{source};
        return search.findValue(path);
    }
};

TEST_F(SearchTests, FindValueInObject)
{
    std::string source = R"({ "foo": 123 })";
    auto token = findValue(source, "/foo");

}

TEST_F(SearchTests, SingleMemberGetPath)
{
    std::string source = R"({ "foo": 123 })";
    auto token = findValue(source, "/foo");

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0)));
}

TEST_F(SearchTests, NestedMemberGetPath)
{
    std::string source = R"({ "foo": { "bar": 123 } })";
    auto token = findValue(source, "/foo/bar");

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0)));
}

TEST_F(SearchTests, MemberInArrayGetPath)
{
    std::string source = R"({ "foo": [1, 2, 3] })";
    auto token = findValue(source, "/foo/1");

    EXPECT_THAT(token, matchesToken(TokenType::Number, "2", Optional(2.0)));
}

TEST_F(SearchTests, MemberInObjectInArrayGetPath)
{
    std::string source = R"({ "foo": [1, { "bar": 123 }, 3] })";
    auto token = findValue(source, "/foo/1/bar");

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0)));
}

TEST_F(SearchTests, SkipObjectMemberWhenNoMatch)
{
    std::string source = R"({ "skip": { "this": "object" }, "foo": 123 })";
    auto token = findValue(source, "/foo");

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0)));
}