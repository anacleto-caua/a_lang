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
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/slint/lib/cmake/Slint")
set(Slint_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/libs/slint/bin/slint-compiler.exe")

find_package(Slint)

add_executable(${APP_NAME} 
    src/main.cpp 
)

# Link libraries
target_link_libraries(${APP_NAME} PRIVATE
    Slint::Slint
)

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
    target_link_libraries(${APP_NAME} pthread dl)
endif()

# Run Target
add_custom_target(run
    COMMAND ${APP_NAME}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${APP_NAME}>
    DEPENDS ${APP_NAME}
    COMMENT "Running ${APP_NAME}..."
    USES_TERMINAL
)