#include <iostream>
#include <patchjson/patchjson.hpp>
#include "args.hpp"
#include "version.hpp"

using namespace patchjson_app;

void printHelp()
{
    std::cout << "Usage: patchjson [options] FILE PATH VALUE" << std::endl;
    std::cout << "Patch JSON file with new values." << std::endl;
    std::cout << std::endl;
    std::cout << "FILE    JSON file to patch." << std::endl;
    std::cout << "PATH    JSON Pointer notation to the value to change (RFC 6901)." << std::endl;
    std::cout << "VALUE   New value to set." << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --version  Display help information" << std::endl;
    std::cout << "  --help  Display help information" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  patchjson file.json /foo/bar 123" << std::endl;
}

int main(int argc, char* argv[])
{
    Args args(argc, argv);

    if (args.help)
    {
        printHelp();
        return 0;
    }
    if (args.version)
    {
        std::cout << "patchjson " << VERSION << std::endl;
        return 0;
    }

    if (not args.file.has_value() or not args.path.has_value() or not args.replacement.has_value())
    {
        std::cerr << "Error: Missing required arguments." << std::endl;
        printHelp();
        return 1;
    }

    try
    {
        patchjson::patchFile(args.file.value(), args.path.value(), args.replacement.value());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
