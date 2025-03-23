add_executable(patchjson_app
    src/main.cpp
    src/args.cpp
    src/args.hpp
    src/version.hpp
)
target_link_libraries(patchjson_app patchjson)
target_compile_features(patchjson_app PRIVATE cxx_std_23)
set_target_properties(patchjson_app
    PROPERTIES
    OUTPUT_NAME patchjson
)
