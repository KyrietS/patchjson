#include <gtest/gtest.h>
#include "patchjson/patchjson.hpp"


TEST(PatchjsonTests, PatchNumber)
{
    std::string content = R"({ "number": 123 })";
    std::string patchedContent = patchjson::patchContent(content, "/number", "456");
    EXPECT_EQ(patchedContent, R"({ "number": 456 })");
}
