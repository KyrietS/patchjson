#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "json_object.hpp"
#include "utils/token_utils.hpp"

using namespace testing;
using namespace json_manip;

namespace match
{
Matcher<const JsonValue&> IsJsonObject()
{
    return Property(&JsonValue::isObject, true);
}
}

JsonValue makeValue(double value)
{
    return JsonValue{Token{TokenType::Number, std::to_string(value), value, 0}};
}
JsonValue makeValue(const std::string& value)
{
    return JsonValue{Token{TokenType::String, "\"" + value + "\"", value, 0}};
}
JsonValue makeValue(const char* value)
{
    return makeValue(std::string{value});
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

TEST(JsonValueTests, StringValue)
{
    Token stringToken{TokenType::String, "\"foo\"", "foo", 0};
    JsonValue value{stringToken};
    EXPECT_TRUE(value.isString());
    EXPECT_EQ(value, "foo");
}

TEST(JsonValueTests, NumberValue)
{
    Token numberToken{TokenType::Number, "0.5", 0.5, 0};
    JsonValue value{numberToken};
    EXPECT_TRUE(value.isNumber());
    EXPECT_EQ(value, 0.5);
}

TEST(JsonValueTests, BooleanValue)
{
    Token trueToken{TokenType::True, "true", true, 0};
    JsonValue value{trueToken};
    EXPECT_TRUE(value.isBoolean());
    EXPECT_EQ(value, true);
}

TEST(JsonValueTests, NullValue)
{
    Token nullToken{TokenType::Null, "null", {}, 0};
    JsonValue value{nullToken};
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(value, nullptr);
}

TEST(JsonValueTests, ObjectValue)
{
    Token openBraceToken{TokenType::OpenBrace, "{", {}, 0};
    JsonValue value{openBraceToken, std::make_unique<JsonObject>()};
    ASSERT_TRUE(value.isObject());
    EXPECT_THAT(value.getObject(), IsEmpty());
}

TEST(JsonObjectTests, EmptyObject)
{
    JsonObject object{};
    EXPECT_THAT(object, IsEmpty());
    EXPECT_THAT(object, SizeIs(0));
}

TEST(JsonObjectTests, ObjectWithMemberValues)
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
}

TEST(JsonObjectTests, ObjectWithMemberObject)
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

TEST(JsonObjectTests, ObjectArray)
{
    JsonObject array{};
    array.add("key1", makeValue("foo"));
    array.add("key2", makeValue(0.5));
    array.add("key3", makeValue(true));
    array.add("key4", makeValue(nullptr));

    for(const auto&[key, value] : array)
    {
        EXPECT_FALSE(value.isObject());
        std::cout << "key: " << key << ", value: " << value << std::endl;
    }

    EXPECT_THAT(array, Not(IsEmpty()));
    EXPECT_THAT(array, SizeIs(4));

    EXPECT_EQ(array.at(0), "foo");
    EXPECT_EQ(array.at(1), 0.5);
    EXPECT_EQ(array.at(2), true);
    EXPECT_EQ(array.at(3), nullptr);
}