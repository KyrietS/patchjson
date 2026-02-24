#include <gtest/gtest.h>
#include "patchjson/json_path.hpp"

using namespace testing;
using namespace patchjson;

TEST(JsonPathTests, EmptyPath)
{
    JsonPath path{std::vector<std::string>{}};
    EXPECT_EQ(path.toString(), "");
}

TEST(JsonPathTests, SingleElementPath)
{
    JsonPath path{std::vector<std::string>{"foo"}};
    EXPECT_EQ(path.toString(), "/foo");
}

TEST(JsonPathTests, MultiElementPath)
{
    JsonPath path{{"foo", "bar", "baz"}};
    EXPECT_EQ(path.toString(), "/foo/bar/baz");
}

TEST(JsonPathTests, SpanConversion)
{
    JsonPath path{{"foo", "bar", "baz"}};
    std::span<const std::string> span = path;
    EXPECT_EQ(span.size(), 3);
    EXPECT_EQ(span[0], "foo");
    EXPECT_EQ(span[1], "bar");
    EXPECT_EQ(span[2], "baz");
}

TEST(JsonPathTests, CreateFromCString)
{
    JsonPath path = "/foo/bar/baz";
    EXPECT_EQ(path.toString(), "/foo/bar/baz");
}