cmake_minimum_required(VERSION 3.10)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.16.0
)

set(TEST_TARGET patchjson_tests)

project(${TEST_TARGET})
set(CMAKE_CXX_STANDARD 23)

add_executable(${TEST_TARGET}
    patchjson_tests.cpp
    json_object_tests.cpp
    json_path_tests.cpp
    document_tests.cpp
    lexer_tests.cpp
    parser_tests.cpp

    utils/token_utils.cpp
)

target_include_directories(${TEST_TARGET} PRIVATE ../src) # Allow testing of internal functions
target_link_libraries(${TEST_TARGET} patchjson)

# Google Test
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE) # Windows clang fix
FetchContent_MakeAvailable(googletest)
target_link_libraries(${TEST_TARGET} gtest gtest_main gmock)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})
