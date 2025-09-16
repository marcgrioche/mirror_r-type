cmake_minimum_required(VERSION 3.16)
project(r-type)

# Include CPM.cmake for dependency management
include(cmake/CPM.cmake)

# Add dependencies using CPM
CPMAddPackage(
    NAME asio
    GITHUB_REPOSITORY chriskohlhoff/asio
    GIT_TAG asio-1-30-2
    OPTIONS
        "ASIO_STANDALONE ON"
)

# Make asio available to all targets
if(asio_ADDED)
    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
    target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

    # Add threading support for asio
    find_package(Threads REQUIRED)
    target_link_libraries(asio INTERFACE Threads::Threads)
endif()

add_subdirectory(client)
add_subdirectory(server)
