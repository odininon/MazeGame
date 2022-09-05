//
// Created by Michael Stengel on 9/4/2022.
//

#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifdef ENABLE_STEAM
#include "steam/steam_api.h"
#endif

class Camera {
 public:
  Camera() {
    Position = glm::vec3(0, 5, 0);
    Up = glm::vec3(0, 1, 0);
    Front = glm::vec3(0, 0, -1);
    WorldUp = Up;

    updateCameraVectors();

#ifdef ENABLE_STEAM
    SteamInput()->RunFrame();

    InputHandle_t* inputHandles = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
    SteamInput()->GetConnectedControllers(inputHandles);

    inputHandle = inputHandles[0];
    gameSetHandle = SteamInput()->GetActionSetHandle("InGameControls");

    moveHandle = SteamInput()->GetAnalogActionHandle("Move");
    cameraHandle = SteamInput()->GetAnalogActionHandle("Camera");
#endif
  }

  void update(float deltaTime, GLFWwindow* window, const bool pBoolean[1024]);

  glm::mat4 getViewMatrix() {
    // Camera matrix
    glm::mat4 View = glm::lookAt(Position, Position + Front, Up);

    return m_Projection * View;
  }

 private:
  void updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(
        Front, WorldUp));  // normalize the vectors, because their length gets
                           // closer to 0 the more you look up or down which
                           // results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
  }

 private:
#ifdef ENABLE_STEAM
  InputAnalogActionHandle_t moveHandle;
  InputAnalogActionHandle_t cameraHandle;
  InputHandle_t inputHandle;
  InputActionSetHandle_t gameSetHandle;
#endif

  float Yaw = 0.0f;
  float Pitch = 0.0f;
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  glm::mat4 m_Projection = glm::perspective(
      glm::radians(45.0f), (float)800 / (float)600, 0.1f, 1000.0f);
};
