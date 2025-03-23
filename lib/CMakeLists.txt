file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)
add_library(patchjson STATIC ${SOURCES})
target_include_directories(patchjson PUBLIC include/)
target_compile_features(patchjson PRIVATE cxx_std_23)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
