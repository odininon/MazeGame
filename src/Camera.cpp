//
// Created by Michael Stengel on 9/4/2022.
//

#include "Camera.h"

#include <GLFW/glfw3.h>

const float cameraSpeed = 10.0f;

float lastX = 1280 / 2.0f;
bool firstMouse = true;

float mouseSens = 0.05f;

void Camera::update(float deltaTime, GLFWwindow* window) {
#ifdef ENABLE_STEAM
  SteamInput()->RunFrame();

  SteamInput()->ActivateActionSet(inputHandle, gameSetHandle);

  InputAnalogActionData_t movementData =
      SteamInput()->GetAnalogActionData(inputHandle, moveHandle);

  InputAnalogActionData_t cameraData =
      SteamInput()->GetAnalogActionData(inputHandle, cameraHandle);
#endif

  GLdouble xPosIn;
  glfwGetCursorPos(window, &xPosIn, nullptr);

  float xpos = static_cast<float>(xPosIn);

  if (firstMouse) {
    lastX = xpos;

    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  lastX = xpos;

  xoffset *= mouseSens;

  Yaw += xoffset;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Position += cameraSpeed * Right * deltaTime;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Position -= cameraSpeed * Right * deltaTime;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Position += cameraSpeed * Front * deltaTime;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Position -= cameraSpeed * Front * deltaTime;
  }

#ifdef ENABLE_STEAM
  if (movementData.bActive) {
    Position += cameraSpeed * Front * deltaTime * movementData.y;
    Position += cameraSpeed * Right * deltaTime * movementData.x;
  }

  if (cameraData.bActive) {
    Yaw += cameraData.x * mouseSens;
  }
#endif

  updateCameraVectors();
}
