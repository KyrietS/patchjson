add_executable(patchjson
    src/main.cpp
    src/args.cpp
    src/args.hpp
    src/version.hpp
)
target_link_libraries(patchjson libpatchjson)
target_compile_features(patchjson PRIVATE cxx_std_23)
