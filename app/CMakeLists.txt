add_executable(patchjson src/main.cpp)
target_link_libraries(patchjson libpatchjson)
target_compile_features(patchjson PRIVATE cxx_std_23)
