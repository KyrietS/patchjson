# patchjson

patchjson is a small utility for modifying values in JSON files. It does not change the structure or formatting of the JSON file. It only changes the values that are already present.

## Usage
```
Usage: patchjson [options] FILE PATH VALUE
Patch JSON file with new values.

FILE    JSON file to patch.
PATH    JSON Pointer notation to the value to change (RFC 6901).
VALUE   New value to set.

Options:
  -h, --help  Display help information

Example:
  patchjson file.json /foo/bar 123
```


## Using library patchjson in your project

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
- `app/` - Source files for patchjson. **(Executable)**
- `lib/` - Source files for patchjson. **(Static library)**

**Unit tests**
- `lib/tests/` - Source files for patchjson_tests. **(Executable)**

## Tests
```
cmake .. -DBUILD_TESTS=ON
```

## License
Copyright © 2025 KyrietS\
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.