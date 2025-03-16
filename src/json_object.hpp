#pragma once
#include <string>
#include <ostream>
#include <cstddef>
#include <vector>
#include <memory>
#include <utility>
#include <variant>
#include <string_view>
#include <span>
#include "token.hpp"
#include "json_path.hpp"

namespace json_manip
{
    class JsonObject;

    using JsonNumberType = double;
    using JsonStringType = std::string;
    using JsonBooleanType = bool;
    using JsonNullType = std::nullptr_t;
    using JsonValueType = std::variant<JsonNumberType, JsonStringType, JsonBooleanType, JsonNullType, std::unique_ptr<JsonObject>>;


    class JsonValue
    {
    public:
        JsonValue(Token token);
        JsonValue(Token token, std::unique_ptr<JsonObject> object);
        void setLocation(std::string_view location) { this->location = location; }
        void setParent(JsonValue* parent) { this->parent = parent; }

        bool isString() const { return token.type == TokenType::String; }
        bool isNumber() const { return token.type == TokenType::Number; }
        bool isBoolean() const { return token.type == TokenType::True or token.type == TokenType::False; }
        bool isNull() const { return token.type == TokenType::Null; }
        bool isObject() const { return token.type == TokenType::OpenBrace and object != nullptr; }
        bool isArray() const { return token.type == TokenType::OpenBracket and object != nullptr; }
        bool isObjectOrArray() const { return isObject() or isArray(); }

        JsonStringType getString() const { expect(isString()); return std::get<std::string>(token.literal.value()); }
        JsonNumberType getNumber() const { expect(isNumber()); return std::get<double>(token.literal.value()); }
        JsonBooleanType getBoolean() const { expect(isBoolean()); return std::get<bool>(token.literal.value()); }
        JsonNullType getNull() const { expect(isNull()); return nullptr; }
        const JsonObject& getObject() const { expect(isObject() or isArray()); return *object; }
        JsonObject& getObject() { expect(isObject() or isArray()); return *object; }

        bool operator==(const char* rhs) const { return isString() && getString() == rhs; }
        bool operator==(const JsonStringType& rhs) const { return isString() && getString() == rhs; }
        bool operator==(const JsonNumberType& rhs) const { return isNumber() && getNumber() == rhs; }
        bool operator==(const JsonBooleanType& rhs) const { return isBoolean() && getBoolean() == rhs; }
        bool operator==(const JsonNullType& rhs) const { return isNull(); }

        JsonPath path() const;
        const JsonValue& find(std::span<const std::string> path) const;

        std::string toString() const;

        Token token;

        std::string_view location{};
        JsonValue* parent{};

    private:
        std::unique_ptr<JsonObject> object{};

        void expect(bool condition) const
        {
            if (not condition)
            {
                throw std::runtime_error("JsonValue expectation failed");
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const JsonValue& value);
    };


    struct JsonObjectMemberType
    {
        std::string key;
        JsonValue value;
    };
    using JsonObjectMembersType = std::vector<JsonObjectMemberType>;
    using JsonArrayType = std::vector<JsonValue>;


    class JsonObject
    {
    public:
        JsonObject() = default;

        const JsonValue& at(const std::string_view key) const;
        const JsonValue& at(size_t index) const;
        void add(std::string key, JsonValue value);

        const JsonValue& find(std::span<const std::string> path) const;

        size_t size() const { return members.size(); }
        bool empty() const { return members.empty(); }
        auto begin() { return members.begin(); }
        auto end() { return members.end(); }

        Token beginToken{};
        Token endToken{};

    private:
        JsonObjectMembersType members;
    };
}
