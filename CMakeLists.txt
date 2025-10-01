cmake_minimum_required(VERSION 3.16)
project(r-type)

file(GLOB_RECURSE SERVER_SOURCES src/*.cpp ../shared/*.cpp)
include_directories(src include ../shared/include)

# Add custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
