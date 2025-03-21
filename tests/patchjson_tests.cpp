#include <gtest/gtest.h>
#include "patchjson/patchjson.hpp"


TEST(PatchjsonTests, PatchNumber)
{
    std::string content = R"({ "number": 123 })";
    std::string patchedContent = patchjson::patchContent(content, "/number", "456");
    EXPECT_EQ(patchedContent, R"({ "number": 456 })");
}

// TODO: Write a proper big json file test (benchmark)
TEST(PatchjsonTests, DISABLED_PatchNumberInFile)
{
    std::string path = "5mb.json";
    patchjson::patchFile(path, "/arr/15839/version", "456");
}
