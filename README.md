# r-type

## Overview

A C++17 client-server project built with CMake.

## Prerequisites

- CMake ≥ 3.16
- C++17 compiler (g++, clang++)
- Git (optional)

## Build Instructions

Clone the repository using git and proceed with the following command inside the cloned repository.

```sh
cmake -S . -B build
cmake --build build
```

### Compilation on Windows

To compile your program on windows, make sure you have the following installed:

- [CMake](https://cmake.org/download/) choose the `cmake-4.1.2-windows-x86_64.msi` installer
- [Visual Studio](https://visualstudio.microsoft.com/downloads/), the community edition is fine.
  Make sure you install the “Desktop development with C++” workload during setup

On the terminal on your machine, run the following commands inside the repository you've cloned:

```sh
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

This will take some time if you're building for the first time.

This will generate `r-type_client` and `r-type_server` binaries in the `build/` directory for `linux` users and in the `build/Release/` directory for windows users.

## Run Instructions

### Linux

```sh
./build/r-type_server
./build/r-type_client
```

### Windows

```sh
./build/Release/r-type_server.exe
./build/Release/r-type_client.exe
```

## Debug Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
or
cmake --build build --config Debug
```

## Troubleshooting

- Ensure CMake and a C++17 compiler are installed.
- If you encounter errors, check your compiler version and CMake version.

## Contributing

Feel free to open issues or submit pull requests.
