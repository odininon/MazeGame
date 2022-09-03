//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

class GLFWwindow;

class GameManager {
 public:
  GameManager();
  ~GameManager() = default;

  void run();

 private:
  GLFWwindow *window;
  unsigned int shaderProgram;
};
