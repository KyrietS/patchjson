#pragma once
#include <string_view>
#include <memory>
#include <unordered_map>
#include "patchjson/json_object.hpp"
#include "patchjson/json_path.hpp"

namespace patchjson
{
class JsonDocument
{
public:
    JsonDocument() = default;

    /// Loads a JSON document from a JSON source.
    /// Note: this method will invalidate a reference to JsonObject returned by json().
    void load(std::string source);

    /// Gets JSON object from parsed document.
    /// Note: reference returned by this method is valid only until the next call to load() or replace().
    const JsonObject& json() const;

    /// Replace number with a new value.
    /// Note: this method will invalidate a reference to JsonObject returned by json().
    void replace(const JsonPath& path, int newValue);

    /// Checks if the document contains a replacable value at the specified path.
    bool replacable(const JsonPath& path) const;

    /// Gets the source of the document.
    const std::string& content() const;

private:
    std::string source;
    JsonObject object;
};
}