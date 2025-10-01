cmake_minimum_required(VERSION 3.16)

project(r-type_server)

file(GLOB SERVER_SOURCES src/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES ../shared/src/*.cpp)
include_directories(src include ../shared/include ../shared/src ../shared/src/ecs ../shared/src/ecs/components)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES})

if(WIN32)
    target_link_libraries(r-type_server PRIVATE ws2_32)
endif()

set_target_properties(r-type_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific flags
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
