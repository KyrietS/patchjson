# patchjson

patchjson is a small utility to modify values in JSON files. It does not change the structure nor the formatting of the JSON file. Only the values that are already present can be changed.

## Getting started

Use FetchContent to include patchjson in your project.
```cmake
include(FetchContent)
FetchContent_Declare(
    patchjson
    GIT_REPOSITORY https://github.com/KyrietS/patchjson.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(patchjson)
target_link_libraries(your_target patchjson)
```

or include it as a subdirectory after cloning the repository.
```cmake
add_subdirectory(patchjson)
target_link_libraries(your_target patchjson)
```

## Tests
```
cmake .. -DTESTS=ON
```

## License
Copyright © 2025 KyrietS\
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.