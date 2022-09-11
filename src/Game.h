//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <memory>

#include "Camera.h"
#include "DefaultScene.h"
#include "Maze.h"
#include "Scene.h"
#include "steam/steam_api.h"

class Game {
 public:
  unsigned int Width, Height;

  Game(unsigned int width, unsigned int height);
  ~Game();

  void Init(GLFWwindow* window);
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();

  bool checkWallCollision(const glm::vec3& player) const;

 private:
  GLFWwindow* m_Window{};

  std::unique_ptr<DefaultScene> scene;
  std::unique_ptr<Camera> camera;

  glm::vec2 playerVelocity{};
  glm::vec3 playerRotation{};

  bool firstMouse = true;
  float lastX{};

  InputAnalogActionHandle_t moveHandle;
  InputAnalogActionHandle_t cameraHandle;
  InputHandle_t inputHandle;
  InputActionSetHandle_t gameSetHandle;
};
