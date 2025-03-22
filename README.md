# patchjson

patchjson is a small utility to modify values in JSON files. It does not change the structure nor the formatting of the JSON file. Only the values that are already present can be changed.

## Usage
```
Usage: patchjson [options] FILE PATH VALUE [PATH VALUE ...]
Patch JSON file with new values.

  FILE    JSON file to patch.
  PATH    JSON Pointer notation to the value to change (RFC 6901).
  VALUE   New value to set.

Example:
  patchjson file.json /key1 111 /key2/key3 222
```


## Using library libpatchjson in your project

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

## Project structure
- **(Library)** libpatchjson: utilities for patching JSON files.
- **(Executable)** patchjson: executable for libpatchjson_cli.

**Tests**
- **(Executable)** patchjson_tests: Tests for libpatchjson.

## Tests
```
cmake .. -DBUILD_TESTS=ON
```

## License
Copyright © 2025 KyrietS\
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.