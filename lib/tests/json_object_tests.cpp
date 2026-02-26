#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <list>

#include "patchjson/json_object.hpp"
#include "utils/token_utils.hpp"

using namespace testing;
using namespace patchjson;

namespace match
{
Matcher<const JsonValue&> IsJsonObject()
{
    return Property(&JsonValue::isObject, true);
}
}

struct JsonValueTests : public Test
{
    JsonValue makeValue(double value)
    {
        lexemes.emplace_back(std::to_string(value));
        return JsonValue{Token{TokenType::Number, lexemes.back(), value, 0}};
    }
    JsonValue makeValue(const char* value)
    {
        lexemes.emplace_back("\"" + std::string{value} + "\"");
        return JsonValue{Token{TokenType::String, lexemes.back(), value, 0}};
    }
    JsonValue makeValue(bool value)
    {
        return JsonValue{Token{value ? TokenType::True : TokenType::False, value ? "true" : "false", value, 0}};
    }
    JsonValue makeValue(std::nullptr_t)
    {
        return JsonValue{Token{TokenType::Null, "null", {}, 0}};
    }
    JsonValue makeValue(std::unique_ptr<JsonObject> value)
    {
        return JsonValue{token_utils::openBraceToken, std::move(value)};
    }

    std::list<std::string> lexemes;
};

TEST_F(JsonValueTests, StringValue)
{
    Token stringToken{TokenType::String, "\"foo\"", "foo", 0};
    JsonValue value{stringToken};
    EXPECT_TRUE(value.isString());
    EXPECT_EQ(value, "foo");
}

TEST_F(JsonValueTests, NumberValue)
{
    Token numberToken{TokenType::Number, "0.5", 0.5, 0};
    JsonValue value{numberToken};
    EXPECT_TRUE(value.isNumber());
    EXPECT_EQ(value, 0.5);
}

TEST_F(JsonValueTests, BooleanValue)
{
    Token trueToken{TokenType::True, "true", true, 0};
    JsonValue value{trueToken};
    EXPECT_TRUE(value.isBoolean());
    EXPECT_EQ(value, true);
}

TEST_F(JsonValueTests, NullValue)
{
    Token nullToken{TokenType::Null, "null", {}, 0};
    JsonValue value{nullToken};
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(value, nullptr);
}

TEST_F(JsonValueTests, ObjectValue)
{
    Token openBraceToken{TokenType::OpenBrace, "{", {}, 0};
    JsonValue value{openBraceToken, std::make_unique<JsonObject>()};
    ASSERT_TRUE(value.isObject());
    EXPECT_THAT(value.getObject(), IsEmpty());
}

struct JsonObjectTests : JsonValueTests {};

TEST_F(JsonObjectTests, EmptyObject)
{
    JsonObject object{};
    EXPECT_THAT(object, IsEmpty());
    EXPECT_THAT(object, SizeIs(0));
}

TEST_F(JsonObjectTests, ObjectWithMemberValues)
{
    JsonObject object{};
    object.add("key1", makeValue("foo"));
    object.add("key2", makeValue(0.5));
    object.add("key3", makeValue(true));
    object.add("key4", makeValue(nullptr));

    EXPECT_THAT(object, Not(IsEmpty()));
    EXPECT_THAT(object, SizeIs(4));

    const JsonValue& value = object.at("key1");

    EXPECT_EQ(object.at("key1"), "foo");
    EXPECT_EQ(object.at("key2"), 0.5);
    EXPECT_EQ(object.at("key3"), true);
    EXPECT_EQ(object.at("key4"), nullptr);

    EXPECT_TRUE(object.has("key1"));
    EXPECT_FALSE(object.has("nonexistent"));
}

TEST_F(JsonObjectTests, ObjectIsIterable)
{
    JsonObject object{};
    object.add("key1", makeValue("foo"));
    object.add("key2", makeValue(0.5));
    object.add("key3", makeValue(true));
    object.add("key4", makeValue(nullptr));

    std::vector<std::string> keys;
    for (auto& [key, value] : object)
    {
        keys.push_back(key);
    }

    EXPECT_THAT(keys, ElementsAre("key1", "key2", "key3", "key4"));
}

TEST_F(JsonObjectTests, ConstObjectIsIterable)
{
    JsonObject object{};
    object.add("key1", makeValue("foo"));
    object.add("key2", makeValue(0.5));
    object.add("key3", makeValue(true));
    object.add("key4", makeValue(nullptr));

    const JsonObject& constObject = object;

    std::vector<std::string> keys;
    for (const auto& [key, value] : constObject)
    {
        keys.push_back(key);
    }

    EXPECT_THAT(keys, ElementsAre("key1", "key2", "key3", "key4"));
}

TEST_F(JsonObjectTests, ObjectWithMemberObject)
{
    JsonObject nestedObject{};
    nestedObject.add("key", makeValue(123.0));

    JsonObject object{};
    object.add("nested", makeValue(std::make_unique<JsonObject>(std::move(nestedObject))));

    EXPECT_THAT(object, Not(IsEmpty()));
    EXPECT_THAT(object, SizeIs(1));
    ASSERT_THAT(object.at("nested"), match::IsJsonObject());
    EXPECT_EQ(object.at("nested").getObject().at("key"), 123.0);
}

TEST_F(JsonObjectTests, ObjectArray)
{
    JsonObject array{};
    array.add("1", makeValue("foo"));
    array.add("2", makeValue(0.5));
    array.add("3", makeValue(true));
    array.add("4", makeValue(nullptr));

    EXPECT_THAT(array, Not(IsEmpty()));
    EXPECT_THAT(array, SizeIs(4));

    EXPECT_EQ(array.at(0), "foo");
    EXPECT_EQ(array.at(1), 0.5);
    EXPECT_EQ(array.at(2), true);
    EXPECT_EQ(array.at(3), nullptr);

    EXPECT_TRUE(array.has("1"));
    EXPECT_FALSE(array.has("nonexistent"));
}