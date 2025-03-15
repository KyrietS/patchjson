# json_manip

json_manip is a simple library for manipulating values in JSON files. It does not alter the structure nor the formatting of the JSON file. Only the values that are already present can be changed.

## Getting started

Use FetchContent to include json_manip in your project.
```cmake
include(FetchContent)
FetchContent_Declare(
    json_manip
    GIT_REPOSITORY https://github.com/KyrietS/json_manip.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(json_manip)
target_link_libraries(your_target json_manip)
```

or include it as a subdirectory after cloning the repository.
```cmake
add_subdirectory(json_manip)
target_link_libraries(your_target json_manip)
```

## Tests
```
cmake .. -DTESTS=ON
```

## License
Copyright © 2025 KyrietS\
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.