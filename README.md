# Slint C++ Template

A template for a C++ application that's using [Slint](https://slint.dev) for the user interface and CMake for the build system.

## About

    My version of the slint template [---> at <---][ZIP archive of this repository](https://github.com/slint-ui/slint-cpp-template)

## Prerequisites
    vscode(recommended)
    cmake
    msvc

## Usage

1. Download and extract the [ZIP archive of this repository](https://github.com/slint-ui/slint-cpp-template/archive/refs/heads/main.zip).
2. Rename the extracted directory and change into it:
    ```
    mv slint-cpp-template-main my-project
    cd my-project
    ```
3. Configure with CMake
   ```
   mkdir build
   cmake -B build
   ```
4. Build with CMake
   ```
   cmake --build build
   ```
5. Run the application binary
    * Linux/macOS:
        ```
        ./build/my_application
        ```
    * Windows:
        ```
        build\my_application.exe
        ```
