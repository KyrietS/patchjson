#include "printer.hpp"

namespace patchjson
{
    void Printer::print(std::ostream& os)
    {
        indentLevel = 0;
        printObject(os, object);
        os << "\n";
    }

    void Printer::printObject(std::ostream& os, const JsonObject& objectToPrint)
    {
        bool first = true;
        os << "{";
        indentLevel += 1;
        for (const auto& [key, value] : objectToPrint)
        {
            if (first)
            {
                os << "\n";
            }
            else
            {
                os << ",\n";
            }
            os << std::string(indentLevel * indentSize, ' ') << "\"" << key << "\": ";
            printValue(os, value);
            first = false;
        }
        os << "\n";
        indentLevel -= 1;
        os << std::string(indentLevel * indentSize, ' ') << "}";
    }

    void Printer::printArray(std::ostream& os, const JsonObject& arrayToPrint)
    {
        bool first = true;
        os << "[";
        indentLevel += 1;
        for (const auto& [index, value] : arrayToPrint)
        {
            if (first)
            {
                os << "\n";
            }
            else
            {
                os << ",\n";
            }
            os << std::string(indentLevel * indentSize, ' ');
            printValue(os, value);
            first = false;
        }
        os << "\n";
        indentLevel -= 1;
        os << std::string(indentLevel * indentSize, ' ') << "]";
    }

    void Printer::printValue(std::ostream& os, const JsonValue& valueToPrint)
    {
        if (valueToPrint.isObject()) {
            printObject(os, valueToPrint.getObject());
        } else if (valueToPrint.isArray()) {
            printArray(os, valueToPrint.getObject());
        } else {
            os << valueToPrint.token.lexeme;
        }
    }
}