//
// Created by Michael Stengel on 9/4/2022.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera {
 public:
  explicit Camera(glm::vec3 &position, glm::vec3 &forward, glm::mat4 &projection)
      : m_Position(position),
        m_ForwardDirection(forward),
        m_Projection(projection),
        m_RightDirection(glm::normalize(glm::cross(m_ForwardDirection, m_UpDirection))) {
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix() const {
    return m_Projection * glm::lookAt(m_Position, m_Position + m_ForwardDirection, m_UpDirection);
  }

  glm::vec3 GetPosition() const {
    return m_Position;
  }

  void AddVelocity(glm::vec2 &velocity);
  void AddRotation(glm::vec3 &rotation);

  const glm::vec3 &getMPosition() const;
  void setMPosition(const glm::vec3 &mPosition);

 private:
  void updateCameraVectors();

 private:
  glm::mat4 m_View{};
  glm::mat4 m_Projection;

  glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 m_ForwardDirection;
  glm::vec3 m_RightDirection;

  glm::vec3 m_Position;
  glm::vec3 m_Rotation{};  // Yaw, Pitch, Roll
};
