file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)
add_library(libpatchjson STATIC ${SOURCES})
target_include_directories(libpatchjson PUBLIC include/)
target_compile_features(libpatchjson PRIVATE cxx_std_23)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
