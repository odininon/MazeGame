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
#include "DefaultScene.h"

class Game {
 public:
  unsigned int Width, Height;

  Game(unsigned int width, unsigned int height);
  ~Game();

  void Init(GLFWwindow* window);
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();

  bool checkWallCollision(glm::vec3& player);

 private:
  GLFWwindow* m_Window{};

  std::unique_ptr<DefaultScene> scene;
  std::unique_ptr<Camera> camera;

  glm::vec2 playerVelocity{};
  glm::vec3 playerRotation{};

  bool firstMouse = true;
  float lastX{};
};
