#include <gtest/gtest.h>
#include "printer.hpp"
#include "lexer.hpp"
#include "parser.hpp"

using namespace testing;
using namespace patchjson;

struct PrinterTests : public Test
{
    JsonObject loadObject(const std::string_view source)
    {
        Lexer lexer(source);
        Parser parser(lexer.tokenize());
        return parser.parse();
    }
};

TEST_F(PrinterTests, PrintEmptyObject)
{
    JsonObject object;
    Printer printer{object, 4};

    std::stringstream ss;
    printer.print(ss);

    EXPECT_EQ(ss.str(), "{\n}\n");
}

TEST_F(PrinterTests, PrintObjectWithMembers)
{
    JsonObject object = loadObject(R"({ "key1": "foo", "key2": 0.5, "key3": true, "key4": null })");

    Printer printer{object, 4};

    std::stringstream ss;
    printer.print(ss);

    std::string expectedString = "{\n"
                                 "    \"key1\": \"foo\",\n"
                                 "    \"key2\": 0.5,\n"
                                 "    \"key3\": true,\n"
                                 "    \"key4\": null\n"
                                 "}\n";
    EXPECT_EQ(ss.str(), expectedString);
}

TEST_F(PrinterTests, PrintObjectWithArray)
{
    JsonObject object = loadObject(R"({ "arr": [1, 2, 3] })");

    Printer printer{object, 4};

    std::stringstream ss;
    printer.print(ss);

    std::string expectedString = "{\n"
                                 "    \"arr\": [\n"
                                 "        1,\n"
                                 "        2,\n"
                                 "        3\n"
                                 "    ]\n"
                                 "}\n";
    EXPECT_EQ(ss.str(), expectedString);
}
