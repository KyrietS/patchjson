#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "document.hpp"

using namespace testing;
using namespace json_manip;

struct DocumentTests : public Test
{
    Document document;
};

TEST_F(DocumentTests, EmptyDocument)
{
    document.load("{}");

    EXPECT_THAT(document.json(), IsEmpty());

    EXPECT_EQ(document.content(), "{}");
}

TEST_F(DocumentTests, DocumentWithOneMember)
{
    document.load(R"({ "foo": 123 })");

    const auto& object = document.json();

    EXPECT_THAT(object, SizeIs(1));
    EXPECT_EQ(object.at("foo"), 123.0);

    EXPECT_EQ(document.content(), R"({ "foo": 123 })");
}

TEST_F(DocumentTests, CheckIfValuesAreReplacable)
{
    document.load(R"({ "number": 123, "boolean": true, "string": "foo", "null": null })");

    EXPECT_TRUE(document.replacable("/number"));
    EXPECT_FALSE(document.replacable("/boolean"));
    EXPECT_FALSE(document.replacable("/string"));
    EXPECT_FALSE(document.replacable("/null"));
    EXPECT_FALSE(document.replacable("/nonexistent/path"));
}

TEST_F(DocumentTests, ReplaceNumberInObject)
{
    document.load(R"({ "number": 123 })");

    document.replace("/number", 456);

    const auto& object = document.json();
    EXPECT_EQ(object.at("number"), 456.0);
    EXPECT_EQ(document.content(), R"({ "number": 456 })");
}

TEST_F(DocumentTests, ReplaceNumberInArray)
{
    document.load(R"({ "array": [1, 2, 3] })");

    document.replace("/array/1", 4);

    const auto& object = document.json();
    EXPECT_EQ(object.find("/array/1"), 4.0);
    EXPECT_EQ(document.content(), R"({ "array": [1, 4, 3] })");
}

TEST_F(DocumentTests, ReplaceNumberWithShorterOne)
{
    document.load(R"({ "number": 1234 })");

    document.replace("/number", 5);

    const auto& object = document.json();
    EXPECT_EQ(object.at("number"), 5.0);
    EXPECT_EQ(document.content(), R"({ "number": 5 })");
}

TEST_F(DocumentTests, ReplaceNumberWithLongerOne)
{
    document.load(R"({ "number": 5 })");

    document.replace("/number", 1234);

    const auto& object = document.json();
    EXPECT_EQ(object.at("number"), 1234.0);
    EXPECT_EQ(document.content(), R"({ "number": 1234 })");
}