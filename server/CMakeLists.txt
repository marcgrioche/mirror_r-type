cmake_minimum_required(VERSION 3.16)

project(r-type_server)

file(GLOB SERVER_SOURCES src/*.cpp)
include_directories(src include)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(r-type_server ${SERVER_SOURCES})

set_target_properties(r-type_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra")
