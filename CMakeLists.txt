cmake_minimum_required(VERSION 3.21)

set(APP_NAME "a_lang")

add_compile_definitions(NOMINMAX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${APP_NAME} LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Architecture Check
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "You are trying to build a 64-bit project with a 32-bit compiler. This project requires 64-bit (x64/amd64).")
endif()

# Setup Slint
set(ENV{SLINT_STYLE} "fluent")

if(WIN32)
    # --- WINDOWS CONFIGURATION ---
    # Define the root path for Windows Slint
    set(SLINT_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/win_slint")
    
    # Point to the .exe compiler
    set(Slint_COMPILER "${SLINT_BASE_PATH}/bin/slint-compiler.exe")

elseif(UNIX)
    # --- LINUX CONFIGURATION ---
    # Define the root path for Linux Slint
    set(SLINT_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/linux_slint")
    
    # Point to the binary (no extension)
    set(Slint_COMPILER "${SLINT_BASE_PATH}/bin/slint-compiler")
    
    # Tell the executable where to look for .so files at runtime (RPATH)
    set(CMAKE_BUILD_RPATH "${SLINT_BASE_PATH}/lib")
endif()

# Add the cmake config path (Works for both as long as the internal structure is standard)
list(APPEND CMAKE_PREFIX_PATH "${SLINT_BASE_PATH}/lib/cmake/Slint")

find_package(Slint)

add_executable(${APP_NAME} 
    src/main.cpp 
)

# Include directories
target_include_directories(${APP_NAME} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src/libs/include # Point to the folder containing json.hpp
)
# Link libraries
target_link_libraries(${APP_NAME} PRIVATE Slint::Slint)

slint_target_sources(${APP_NAME} ui/mother_window.slint)


# On Windows, copy the Slint DLL next to the application binary
if(WIN32)
    add_custom_command(TARGET ${APP_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${APP_NAME}> $<TARGET_FILE_DIR:${APP_NAME}> 
        COMMAND_EXPAND_LISTS
        COMMENT " -- Copying Slint DLLs to output directory..."
    )
endif()

if(UNIX)
    target_link_libraries(${APP_NAME} PRIVATE pthread dl)
endif()

# Run Target
add_custom_target(run
    COMMAND ${APP_NAME}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${APP_NAME}>
    DEPENDS ${APP_NAME}
    COMMENT "Running ${APP_NAME}..."
    USES_TERMINAL
)