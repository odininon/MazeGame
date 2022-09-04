cmake_minimum_required(VERSION 3.21)
project(MazeGameConan)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(MazeGameConan src/main.cpp src/GameManager.cpp src/GameManager.h src/Maze.cpp src/Maze.h)
target_link_libraries(MazeGameConan ${CONAN_LIBS})
