cmake_minimum_required(VERSION 3.11)

project(json_manip)

set(CMAKE_CXX_STANDARD 23)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)
add_library(json_manip STATIC ${SOURCES})
target_include_directories(json_manip PUBLIC include/)

# Option to build tests
option(TESTS "Build tests" ON)

if(TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
