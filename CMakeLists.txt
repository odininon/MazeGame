cmake_minimum_required(VERSION 3.13.2)
project(MazeGame)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

set(CMAKE_CXX_STANDARD 14)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

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
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

target_link_libraries(${PROJECT_NAME} ${STEAMSDK_LIB})

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# glm
include_directories("${LIB_DIR}/glm")

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