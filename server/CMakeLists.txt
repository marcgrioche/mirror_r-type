cmake_minimum_required(VERSION 3.16)

project(r-type_server)

file(GLOB SERVER_SOURCES src/*.cpp db/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES ../shared/src/*.cpp)
include_directories(src db include ../shared/include ../shared/src ../shared/src/ecs ../shared/src/ecs/components ../shared/src/ecs/systems ../shared/src/entities ../shared/src/levels)
include_directories(${CMAKE_SOURCE_DIR})

CPMAddPackage(
        NAME sqlite3
        GITHUB_REPOSITORY "azadkuh/sqlite-amalgamation"
        GIT_TAG "3.38.2"
)

add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES})

if (sqlite3_ADDED)
    message(STATUS "Using sqlite3 from CPM")
    target_link_libraries(r-type_server PRIVATE sqlite3)
else()
    find_package(SQLite3 REQUIRED)
    target_link_libraries(r-type_server PRIVATE SQLite::SQLite3)
endif()

if(WIN32)
    target_link_libraries(r-type_server PRIVATE ws2_32)
endif()

target_link_libraries(r-type_server PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(r-type_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
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
