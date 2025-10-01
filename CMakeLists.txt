cmake_minimum_required(VERSION 3.16)
project(r-type)

# Set CMake policy version to provide compatibility for older third-party CMakeLists
set(CMAKE_POLICY_VERSION 3.16)

# Set minimum policy version to handle older CMake dependencies
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

file(GLOB_RECURSE SERVER_SOURCES src/*.cpp ../shared/src/*.cpp)
include_directories(src include ../shared/include ../shared/src ../shared/src/ecs ../shared/src/ecs/components ../shared/src/ecs/systems ../shared/src/entities ../shared/src/levels)

# Add custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Avoid compiler-specific extensions (use portable -std=c++20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /W4")
    set(CMAKE_CXX_FLAGS_SANITIZER "/Zi /W4 /fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_SANITIZER "-g3 -O1 -Wall -Wextra -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_SANITIZER "-fsanitize=address -fsanitize=undefined")
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Sanitizer" CACHE STRING "Available build configurations" FORCE)



add_subdirectory(client)
add_subdirectory(server)
