//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <memory>

#include "Camera.h"
#include "DefaultScene.h"
#include "Maze.h"
#include "Scene.h"
#include "imgui.h"

#ifndef _DEBUG
#include "steam/steam_api.h"
#endif

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

  void processKey(GLFWwindow* window, int key, int action, int mode);

 private:
  GLFWwindow* m_Window{};

  std::unique_ptr<DefaultScene> scene;
  std::unique_ptr<Camera> camera;

  glm::vec2 playerVelocity{};
  glm::vec3 playerRotation{};

  bool firstMouse = true;
  float lastX{};

  bool captureMouse = true;

#ifndef _DEBUG
  InputAnalogActionHandle_t moveHandle;
  InputAnalogActionHandle_t cameraHandle;
  InputHandle_t inputHandle;
  InputActionSetHandle_t gameSetHandle;
#endif
  void renderGameObjects() const;
  void toggleMouseCapture(bool b);
};
