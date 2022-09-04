//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

#include "Maze.h"
class GLFWwindow;

class GameManager {
 public:
  GameManager();
  ~GameManager() = default;

  void run();

 private:
  GLFWwindow* window;

  Maze* maze;
};
