# SDL2 Installation Guide for R-Type Project

This guide explains how to install SDL2 and its extensions on different platforms for the R-Type project.

## Prerequisites

Make sure you have CMake 3.16+ and a C++17 compatible compiler installed.

## Linux (Ubuntu/Debian)

```bash
# Install SDL2 and extensions
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

# Install pkg-config (usually already installed)
sudo apt install pkg-config

# Build the project
mkdir -p build
cd build
cmake ..
make
```

## Linux (Fedora/CentOS/RHEL)

```bash
# Install SDL2 and extensions
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel

# Or for older versions:
# sudo yum install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel

# Install pkg-config
sudo dnf install pkgconfig

# Build the project
mkdir -p build
cd build
cmake ..
make
```

## Linux (Arch Linux)

```bash
# Install SDL2 and extensions
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer

# pkg-config is usually included in base-devel
sudo pacman -S base-devel

# Build the project
mkdir -p build
cd build
cmake ..
make
```

## macOS

### Option 1: Using Homebrew (Recommended)

```bash
# Install Homebrew if you haven't already
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install SDL2 and extensions
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer

# Install pkg-config
brew install pkg-config

# Build the project
mkdir -p build
cd build
cmake ..
make
```

### Option 2: Using MacPorts

```bash
# Install SDL2 and extensions
sudo port install libsdl2 libsdl2_image libsdl2_ttf libsdl2_mixer

# Install pkg-config
sudo port install pkgconfig

# Build the project
mkdir -p build
cd build
cmake ..
make
```

## Windows

### Option 1: Using vcpkg (Recommended)

```cmd
# Install vcpkg if you haven't already
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install SDL2 and extensions
.\vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows sdl2-mixer:x64-windows

# Build the project
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### Option 2: Manual Installation

1. Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/download-2.0.php)
2. Download SDL2_image, SDL2_ttf, and SDL2_mixer from their respective pages
3. Extract all libraries to a common directory (e.g., `C:/SDL2/`)
4. Set the `SDL2_ROOT` environment variable to point to your SDL2 directory
5. Build:

```cmd
mkdir build
cd build
cmake .. -DSDL2_ROOT=C:/SDL2/
cmake --build . --config Release
```

### Option 3: Using MSYS2/MinGW-w64

```bash
# Update package database
pacman -Syu

# Install SDL2 and extensions
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer

# Install pkg-config
pacman -S mingw-w64-x86_64-pkg-config

# Build the project
mkdir -p build
cd build
cmake .. -G "MinGW Makefiles"
make
```

## Verification

After installation, verify that SDL2 is properly detected:

```bash
cd build
cmake ..
```

You should see output indicating that SDL2 was found. If you encounter any issues, check:

1. That all SDL2 libraries are installed
2. That pkg-config can find the libraries: `pkg-config --libs sdl2`
3. That CMAKE_MODULE_PATH includes the custom cmake directory

## Troubleshooting

### Linux Issues
- If you get "SDL2 not found" errors, ensure pkg-config is installed and can find SDL2: `pkg-config --modversion sdl2`
- Make sure you have both runtime and development packages installed (packages ending in -dev or -devel)

### macOS Issues
- If using Homebrew and getting linking errors, try: `brew link --force sdl2`
- Ensure Xcode command line tools are installed: `xcode-select --install`

### Windows Issues
- If using vcpkg, make sure to specify the correct toolchain file
- When using manual installation, ensure all DLL files are in the same directory as your executable
- For Visual Studio, you might need to set the architecture explicitly: `-A x64`

## Cross-Platform Notes

The CMake configuration automatically detects the platform and uses the appropriate method:
- Linux/macOS: Uses pkg-config
- Windows: Uses find_package with custom modules
- Fallback: Uses the custom FindSDL2.cmake module

This ensures your project builds consistently across all platforms.
