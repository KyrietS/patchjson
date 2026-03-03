#include <gtest/gtest.h>
#include "patchjson/json_path.hpp"

using namespace testing;
using namespace patchjson;

TEST(JsonPathTests, EmptyPath)
{
    JsonPath path1{std::vector<std::string>{}};
    JsonPath path2 = "";
    EXPECT_EQ(path1.toString(), "");
    EXPECT_EQ(path2.toString(), "");
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

TEST(JsonPathTests, AppendSegment)
{
    JsonPath path = "/foo/bar";
    path.append("baz");
    EXPECT_EQ(path.toString(), "/foo/bar/baz");
}

TEST(JsonPathTests, OperatorPlus)
{
    JsonPath path = "/foo/bar";
    JsonPath newPath = path + "baz";
    EXPECT_EQ(newPath.toString(), "/foo/bar/baz");
    EXPECT_EQ(path.toString(), "/foo/bar"); // Original path should remain unchanged
}

TEST(JsonPathTests, EqualityOperator)
{
    JsonPath path1 = "/foo/bar";
    JsonPath path2 = "/foo/bar";
    JsonPath path3 = "/foo/baz";

    EXPECT_TRUE(path1 == path2);
    EXPECT_FALSE(path1 == path3);
}
