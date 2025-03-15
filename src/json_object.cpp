#include "json_object.hpp"
#include <stdexcept>
#include <format>

namespace json_manip
{
    JsonValue::JsonValue(Token token, std::unique_ptr<JsonObject> object) : token{std::move(token)}, object{std::move(object)} {}
    JsonValue::JsonValue(Token token) : JsonValue(token, nullptr) {}

    const JsonValue& JsonObject::at(const std::string& key) const
    {
        auto it = std::find_if(members.begin(), members.end(), [&key](const auto& member)
        {
            return member.key == key;
        });
        if (it == members.end())
        {
            throw std::out_of_range{std::format("Key '{}' not found", key)};
        }
        return it->value;
    }

    const JsonValue& JsonObject::at(size_t index) const
    {
        if (index >= members.size())
        {
            throw std::out_of_range{std::format("Index '{}' out of range", index)};
        }
        return members[index].value;
    }

    std::string JsonValue::toString() const
    {
        if (isString()) return getString();
        if (isNumber()) return std::to_string(getNumber());
        if (isBoolean()) return getBoolean() ? "true" : "false";
        if (isNull()) return "null";
        if (isObject()) return "<object>";
        if (isArray()) return "<array>";
        return "<unknown>";
    }

    std::ostream& operator<<(std::ostream& os, const json_manip::JsonValue& value)
    {
        os << value.toString();
        return os;
    }

}