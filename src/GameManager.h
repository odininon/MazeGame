//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

#include "steam/steam_api.h"

#include "Camera.h"
#include "Maze.h"

class GLFWwindow;

class GameManager {
 public:
  GameManager();
  ~GameManager() = default;

  void run();

 public:
  bool Keys[1024];

 private:
  GLFWwindow* window;

  Camera* camera;
  Maze* maze;
};
