#pragma once
#include <string_view>
#include <memory>
#include <unordered_map>
#include "json_object.hpp"

namespace json_manip
{
class Document
{
public:
    Document() = default;

    /// Loads a JSON document from a JSON source.
    void load(std::string_view source);

    /// Gets JSON object from parsed document.
    const JsonObject& json() const;

    /// Gets the source of the document.
    const std::string& content() const;

private:
    std::string source;
    JsonObject object;
};
}