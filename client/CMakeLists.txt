cmake_minimum_required(VERSION 3.16)

project(r-type_client)

file(GLOB_RECURSE CLIENT_SOURCES src/*.cpp)
include_directories(src src/ecs include)

add_executable(r-type_client ${CLIENT_SOURCES})

# Link SDL2 libraries
if(SDL2_FOUND)
    # Using pkg-config (Linux/macOS with Homebrew)
    target_include_directories(r-type_client PRIVATE ${SDL2_INCLUDE_DIRS})
    target_compile_options(r-type_client PRIVATE ${SDL2_CFLAGS_OTHER})
    target_link_libraries(r-type_client PRIVATE 
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES} 
        ${SDL2_TTF_LIBRARIES} 
        ${SDL2_MIXER_LIBRARIES}
    )
    target_link_directories(r-type_client PRIVATE ${SDL2_LIBRARY_DIRS})
else()
    # Using find_package (Windows/vcpkg or custom installation)
    target_link_libraries(r-type_client PRIVATE 
        SDL2::SDL2 
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
    )
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(r-type_client PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    # Copy SDL2 DLLs to output directory (if using vcpkg or manual installation)
    if(TARGET SDL2::SDL2)
        add_custom_command(TARGET r-type_client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:r-type_client>)
    endif()
endif()

set_target_properties(r-type_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
