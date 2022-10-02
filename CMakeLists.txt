cmake_minimum_required(VERSION 3.13.2)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(MazeGame)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

set(CMAKE_CXX_STANDARD 17)

## Find dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# stb does not have a CMake build, but is header-only
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SOURCES
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.cpp"
        )

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

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS} ${VULKAN_HPP_INCLUDE_DIRS} "${SRC_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${STEAMSDK_LIB} freetype glfw glm::glm imgui::imgui)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:MazeGame>/shaders
        COMMENT "Copying shaders" VERBATIM
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/steam_appid.txt $<TARGET_FILE_DIR:MazeGame>/steam_appid.txt
        COMMENT "Copying Steam Appid" VERBATIM
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/game_actions_1398840.vdf $<TARGET_FILE_DIR:MazeGame>/game_actions_1398840.vdf
        COMMENT "Copying Controller Config" VERBATIM
)

if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/steamworks_sdk/redistributable_bin/win64/steam_api64.dll $<TARGET_FILE_DIR:MazeGame>/steam_api64.dll
            COMMENT "Copying Steam" VERBATIM
    )
endif ()

if (UNIX AND NOT APPLE)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/steamworks_sdk/redistributable_bin/linux64/libsteam_api.so $<TARGET_FILE_DIR:MazeGame>/libsteam_api.so
            COMMENT "Copying Steam" VERBATIM
    )
endif ()