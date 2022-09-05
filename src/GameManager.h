//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "Camera.h"
#include "Maze.h"
#include "steam/steam_api.h"

class GLFWwindow;

class GameManager {
 public:
  GameManager();
  ~GameManager() = default;

  void run();

 private:
  GLFWwindow* window;

  Camera* camera;
  Maze* maze;
};
