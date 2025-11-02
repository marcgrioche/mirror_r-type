# Building and Running r-type for Windows from Ubuntu

This guide explains how to **cross-compile the r-type project for Windows from Ubuntu** and test it using Wine.

---

## Prerequisites

- **Ubuntu system**
- **CMake** (`sudo apt install cmake`)
- **mingw-w64** cross-compiler (`sudo apt install mingw-w64`)
- **Wine** (`sudo apt install wine wine32:i386`)
- **Git** (`sudo apt install git`)

---

## Step 1: Install Required Packages

```sh
sudo apt update
sudo apt install cmake mingw-w64 git wine wine32:i386
```

---

## Step 2: Create a CMake Toolchain File

If a file named `mingw-w64-toolchain.cmake` is not present at the root of your directory,  
create a file named `mingw-w64-toolchain.cmake` in your project root with the following contents:

```cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

---

## Step 3: Configure the Project for Windows

```sh
cmake -DCMAKE_TOOLCHAIN_FILE=mingw-w64-toolchain.cmake -B build-win
```

---

## Step 4: Build the Project

```sh
cmake --build build-win
```

Your Windows executable(s) (e.g., `r-type_client.exe`, `r-type_server.exe`) will be in the `build-win` directory.

---

## Step 5: Copy Required DLLs

Your executable may depend on mingw-w64 runtime DLLs.  
Copy these from `/usr/x86_64-w64-mingw32/bin/` to your build-win directory if needed:

```sh
cp /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll build-win/
cp /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll build-win/
cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll build-win/
```

These files can be located in another directory. To locate these files perform a `tree` command once you are in the  
`/usr/x86_64-w64-mingw32/` directory (for my system it was in the `13-win32` directory). You don't need the `libwinpthread-1.dll` for the compilation.


If you use SDL2, copy the SDL DLLs as well.

---

## Step 6: Test the Executable with Wine

```sh
cd build-win
wine r-type_client.exe
```
or
```sh
wine r-type_server.exe
```

---

## Troubleshooting

- If Wine reports missing DLLs, copy them from your mingw-w64 or SDL2 installation to the executable's directory.
- If you see errors about wine32, run:
  ```sh
  sudo dpkg --add-architecture i386
  sudo apt update
  sudo apt install wine32:i386
  ```
- For code warnings about uninitialized variables, initialize all variables before use for compatibility.
- You may get error about missing linker files while running the `wine` command. To fix that, remove the following directory:

```shell
rm -rf ~/.wine
winecfg
```

And choose `windows11` as the OS and the default settings.

---

## Notes

- This process lets you verify that your code builds and runs for Windows without needing a Windows machine.
