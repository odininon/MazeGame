//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Maze.h"
#include "Scene.h"

class Game {
 public:
  unsigned int Width, Height;

  Game(unsigned int width, unsigned int height);
  ~Game();

  void Init();
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();

 private:
  std::unique_ptr<Scene> scene;
  std::unique_ptr<Camera> camera;
};
