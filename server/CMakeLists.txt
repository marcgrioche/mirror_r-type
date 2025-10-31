cmake_minimum_required(VERSION 3.16)

project(r-type_server)

file(GLOB SERVER_SOURCES src/*.cpp db/*.cpp metrics/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES ../shared/src/*.cpp)
include_directories(src db include metrics ../shared/include ../shared/src ../shared/src/ecs ../shared/src/ecs/components ../shared/src/ecs/systems ../shared/src/entities ../shared/src/levels)
include_directories(${CMAKE_SOURCE_DIR})

CPMAddPackage(
        NAME sqlite3
        GITHUB_REPOSITORY "azadkuh/sqlite-amalgamation"
        GIT_TAG "3.38.2"
)

CPMAddPackage(
        NAME zlib
        GITHUB_REPOSITORY "madler/zlib"
        GIT_TAG "v1.3.1"
)

CPMAddPackage(
        NAME sodium
        GITHUB_REPOSITORY "robinlinden/libsodium-cmake"
        GIT_TAG e5b985ad0dd235d8c4307ea3a385b45e76c74c6a
        OPTIONS
            "SODIUM_DISABLE_TESTS ON"
)

# Set prometheus-cpp options BEFORE adding the package
set(ENABLE_PUSH OFF CACHE BOOL "" FORCE)
set(ENABLE_PULL ON CACHE BOOL "" FORCE)
set(ENABLE_COMPRESSION OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

CPMAddPackage(
     NAME prometheus-cpp
     GIT_REPOSITORY "https://github.com/jupp0r/prometheus-cpp.git"
     GIT_TAG "v1.3.0"
)

# Create a library target from the amalgamation if it was added via CPM
if (sqlite3_ADDED)
    add_library(sqlite3_lib STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
    target_include_directories(sqlite3_lib PUBLIC ${sqlite3_SOURCE_DIR})
    # Add pthread and dl libraries which SQLite needs on Linux
    if(UNIX AND NOT APPLE)
        target_link_libraries(sqlite3_lib PUBLIC pthread dl)
    endif()
endif()

add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES})


if (sqlite3_ADDED)
    message(STATUS "Using sqlite3 from CPM")
    target_link_libraries(r-type_server PRIVATE sqlite3_lib)
else()
    find_package(SQLite3 REQUIRED)
    target_include_directories(r-type_server PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(r-type_server PRIVATE SQLite::SQLite3)
endif()

if (libsodium_ADDED)
    message(STATUS "Using libsodium from CPM")
    target_link_libraries(r-type_server PRIVATE sodium)
else()
    find_package(sodium REQUIRED)
    target_link_libraries(r-type_server PRIVATE sodium)
endif()

if(WIN32)
    target_link_libraries(r-type_server PRIVATE ws2_32)
endif()

target_link_libraries(r-type_server PRIVATE prometheus-cpp::core prometheus-cpp::pull)
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
