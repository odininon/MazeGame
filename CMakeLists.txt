cmake_minimum_required(VERSION 3.21)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(MazeGameConan)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (WIN32)
    find_library(STEAMSDK_LIB NAMES steam_api64 HINTS steamworks_sdk/redistributable_bin/win64/)
endif ()

if (APPLE)
    find_library(STEAMSDK_LIB NAMES steam_api HINTS steamworks_sdk/redistributable_bin/osx)
endif ()

if (UNIX AND NOT APPLE)
    find_library(STEAMSDK_LIB NAMES steam_api HINTS steamworks_sdk/redistributable_bin/linux64)
endif ()

include_directories(steamworks_sdk/public)

add_executable(MazeGameConan src/main.cpp src/GameManager.cpp src/GameManager.h src/Maze.cpp src/Maze.h src/Camera.cpp src/Camera.h)
target_link_libraries(MazeGameConan ${CONAN_LIBS} ${STEAMSDK_LIB})