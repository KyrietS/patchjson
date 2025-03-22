cmake_minimum_required(VERSION 3.11)

project(patchjson)

option(BUILD_APP "Build app" ON)
option(BUILD_TESTS "Build tests" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(lib)

if(BUILD_APP)
    add_subdirectory(app)
endif()
