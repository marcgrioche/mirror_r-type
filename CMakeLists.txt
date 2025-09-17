cmake_minimum_required(VERSION 3.16)
project(r-type)

# Include CPM.cmake for dependency management
include(cmake/CPM.cmake)

add_subdirectory(client)
add_subdirectory(server)
