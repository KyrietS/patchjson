#pragma once
#include "patchjson/json_object.hpp"
#include <ostream>

namespace patchjson
{
    class Printer
    {
    public:
        Printer(JsonObject& object, int indentSize) : object(object), indentSize(indentSize) {}
        void print(std::ostream& os);

    private:
        void printObject(std::ostream& os, const JsonObject& object);
        void printArray(std::ostream& os, const JsonObject& array);
        void printValue(std::ostream& os, const JsonValue& value);

        JsonObject& object;
        int indentSize = 4;
        int indentLevel = 0;
    };
}