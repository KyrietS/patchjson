#include "patchjson/patchjson.hpp"
#include <fstream>
#include <sstream>
#include "document.hpp"

namespace patchjson
{
    void patchFile(const std::filesystem::path& path, const std::string& pathToValue, const std::string& newValue)
    {
        std::ifstream input{path};
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string patchedContent = patchContent(buffer.str(), pathToValue, newValue);
        input.close();

        std::ofstream output{path, std::ios::trunc};
        output << patchedContent;
    }

    std::string patchContent(std::string content, const std::string& pathToValue, const std::string& newValue)
    {
        Document document{};
        document.load(std::move(content));

        JsonPath path{pathToValue};
        int value = std::stoi(newValue);
        document.replace(path, value);

        return document.content(); // inefficient copy
    }
}