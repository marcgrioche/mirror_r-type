cmake_minimum_required(VERSION 3.16)

project(r-type_client)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Collect source files
file(GLOB_RECURSE CLIENT_SOURCES src/*.cpp)


CPMAddPackage(
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.30.0
    OPTIONS
        "SDL2_DISABLE_INSTALL ON"
        "SDL2_DISABLE_UNINSTALL ON"

        "SDL_ALSA OFF"
        "SDL_PULSEAUDIO OFF"
        "SDL_JACK OFF"
        "SDL_PIPEWIRE OFF"
        "SDL_SNDIO OFF"
        "SDL_ESD OFF"
        "SDL_ARTS OFF"

        "SDL_KMSDRM OFF"
        "SDL_DIRECTFB OFF"
        "SDL_RPI OFF"
        "SDL_VIVANTE OFF"

        "SDL_X11 ON"
        "SDL_WAYLAND ON"
        "SDL_OSS ON"

        "SDL_LIBUDEV OFF"
        "SDL_IBUS OFF"
        "SDL_LIBSAMPLERATE OFF"
        "SDL_WAYLAND_LIBDECOR OFF"
)


CPMAddPackage(
    NAME SDL2_image
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG release-2.8.2
    OPTIONS
        "SDL2IMAGE_INSTALL OFF"
        "SDL2IMAGE_SAMPLES OFF"
)


CPMAddPackage(
    NAME SDL2_ttf
    GITHUB_REPOSITORY libsdl-org/SDL_ttf
    GIT_TAG release-2.22.0
    OPTIONS
        "SDL2TTF_INSTALL OFF"
        "SDL2TTF_SAMPLES OFF"
        "SDL2TTF_VENDORED ON"
)


CPMAddPackage(
    NAME SDL2_mixer
    GITHUB_REPOSITORY libsdl-org/SDL_mixer
    GIT_TAG release-2.8.0
    OPTIONS
        "SDL2MIXER_INSTALL OFF"
        "SDL2MIXER_SAMPLES OFF"

        "SDL2MIXER_OPUS OFF"
        "SDL2MIXER_FLUIDSYNTH OFF"
        "SDL2MIXER_WAVPACK OFF"
        "SDL2MIXER_MP3_MPG123 OFF"
        "SDL2MIXER_MP3_DRMP3 ON"
        "SDL2MIXER_VORBIS STB"
        "SDL2MIXER_MOD OFF"
        "SDL2MIXER_MOD_XMP OFF"
        "SDL2MIXER_MOD_MODPLUG OFF"
        "SDL2MIXER_MIDI_FLUIDSYNTH OFF"
        "SDL2MIXER_MIDI_NATIVE OFF"
)

add_executable(r-type_client ${CLIENT_SOURCES})

# Set up include directories with proper hierarchy
target_include_directories(r-type_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src                    # Root source directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs                # ECS core directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/components     # Components directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems        # Systems directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities           # Entities directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/managers           # Managers directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game_handler       # Game handler directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/levels             # Levels directory
)

target_link_libraries(r-type_client PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)


if(WIN32)

    set_target_properties(r-type_client PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:r-type_client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image::SDL2_image>
        $<TARGET_FILE_DIR:r-type_client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:r-type_client>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
        $<TARGET_FILE_DIR:r-type_client>
    )
endif()

set_target_properties(r-type_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
