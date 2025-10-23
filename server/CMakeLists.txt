cmake_minimum_required(VERSION 3.16)

project(r-type_server)

file(GLOB SERVER_SOURCES src/*.cpp db/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES ../shared/src/*.cpp)
include_directories(src db include ../shared/include ../shared/src ../shared/src/ecs ../shared/src/ecs/components ../shared/src/ecs/systems ../shared/src/entities ../shared/src/levels)
include_directories(${CMAKE_SOURCE_DIR})

CPMAddPackage(
    NAME sqlite
    GITHUB_REPOSITORY sqlite/sqlite
    GIT_TAG        version-3.42.1
    OPTIONS        "SQLITE_ENABLE_JSON1 ON" "SQLITE_ENABLE_FTS5 ON"
)

add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES})

if (TARGET sqlite::sqlite3)
    message(STATUS "Using sqlite target: sqlite::sqlite3")
    target_link_libraries(r-type_server PRIVATE sqlite::sqlite3)
elseif (TARGET sqlite3)
    message(STATUS "Using sqlite target: sqlite3")
    add_library(sqlite::sqlite3 ALIAS sqlite3)
    target_link_libraries(r-type_server PRIVATE sqlite::sqlite3)
elseif (TARGET sqlite)
    message(STATUS "Using sqlite target: sqlite")
    add_library(sqlite::sqlite3 ALIAS sqlite)
    target_link_libraries(r-type_server PRIVATE sqlite::sqlite3)
else()
    # Try system package via FindSQLite3 or pkg-config
    find_package(SQLite3 QUIET)
    if (TARGET SQLite::SQLite3)
        message(STATUS "Using system SQLite: SQLite::SQLite3")
        target_link_libraries(r-type_server PRIVATE SQLite::SQLite3)
    else()
        # Last resort: fail with a helpful message
        message(FATAL_ERROR "SQLite package not available. Install sqlite3 development package or adjust CPM tag/FetchContent fallback.")
    endif()
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
