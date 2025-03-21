#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "find.hpp"
#include "utils/token_utils.hpp"

using namespace testing;
using namespace patchjson;
using namespace token_utils;

TEST(FindTests, FindValueInObject)
{
    std::string source = R"({ "foo": 123 })";
    auto token = findValue(source, JsonPath{"/foo"});

}


TEST(FindTests, SingleMemberGetPath)
{
    std::string source = R"({ "foo": 123 })";
    auto token = findValue(source, JsonPath{"/foo"});

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0), 9));
}

TEST(FindTests, NestedMemberGetPath)
{
    std::string source = R"({ "foo": { "bar": 123 } })";
    auto token = findValue(source, JsonPath{"/foo/bar"});

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0), 18));
}

TEST(FindTests, MemberInArrayGetPath)
{
    std::string source = R"({ "foo": [1, 2, 3] })";
    auto token = findValue(source, JsonPath{"/foo/1"});

    EXPECT_THAT(token, matchesToken(TokenType::Number, "2", Optional(2.0), 13));
}

TEST(FindTests, MemberInObjectInArrayGetPath)
{
    std::string source = R"({ "foo": [1, { "bar": 123 }, 3] })";
    auto token = findValue(source, JsonPath{"/foo/1/bar"});

    EXPECT_THAT(token, matchesToken(TokenType::Number, "123", Optional(123.0), 22));
}
