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
