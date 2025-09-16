# r-type

## Overview

A C++17 client-server project built with CMake.

## Prerequisites

- CMake ≥ 3.16
- C++17 compiler (g++, clang++)
- Git (optional)

## Build Instructions

```sh
mkdir -p cmake
wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
cmake -S . -B build
cmake --build build
```

This will generate `r-type_client` and `r-type_server` binaries in the `build/` directory.

## Run Instructions

From the `build/` directory:

```sh
./r-type_server
./r-type_client
```

## Debug Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Troubleshooting

- Ensure CMake and a C++17 compiler are installed.
- If you encounter errors, check your compiler version and CMake version.

## Contributing

Feel free to open issues or submit pull requests.
