cmake_minimum_required(VERSION 3.16)

project(r-type_client)

file(GLOB CLIENT_SOURCES src/*.cpp)
include_directories(src include)

add_executable(r-type_client ${CLIENT_SOURCES})

set_target_properties(r-type_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra")
