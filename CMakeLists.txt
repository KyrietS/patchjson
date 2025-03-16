cmake_minimum_required(VERSION 3.11)

project(patchjson)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)
add_library(patchjson STATIC ${SOURCES})
target_include_directories(patchjson PUBLIC include/)
target_compile_features(patchjson PRIVATE cxx_std_23)

# Option to build tests
option(TESTS "Build tests" ON)

if(TESTS)
    add_subdirectory(tests)
endif()
