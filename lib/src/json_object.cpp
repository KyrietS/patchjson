#include "json_object.hpp"
#include <stdexcept>
#include <format>
#include <cassert>

namespace patchjson
{
    JsonValue::JsonValue(Token token, std::unique_ptr<JsonObject> object) : token{std::move(token)}, object{std::move(object)} {}
    JsonValue::JsonValue(Token token) : JsonValue(token, nullptr) {}

    const JsonValue& JsonObject::at(std::string_view key) const
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

    void JsonObject::add(std::string key, JsonValue value)
    {
        members.push_back({std::move(key), std::move(value)});
    }

    JsonPath JsonValue::path() const
    {
        std::vector<std::string> path;
        for (const JsonValue* current = this; current != nullptr; current = current->parent)
        {
            path.push_back(std::string{current->location});
        }
        std::reverse(path.begin(), path.end());
        return JsonPath{path};
    }

    const JsonValue& JsonValue::find(std::span<const std::string> path) const
    {
        return path.empty() ? *this : getObject().find(path);
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

    const JsonValue& JsonObject::find(std::span<const std::string> path) const
    {
        assert(not path.empty());
        const std::string_view location = path.front();
        path = path.subspan(1);
        return at(location).find(path);
    }

    std::ostream& operator<<(std::ostream& os, const patchjson::JsonValue& value)
    {
        os << value.toString();
        return os;
    }

}