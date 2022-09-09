//
// Created by Michael Stengel on 9/4/2022.
//

#include "Camera.h"

const float cameraSpeed = 15.0f;

void Camera::updateCameraVectors() {
  // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
  front.y = sin(glm::radians(m_Rotation.y));
  front.z = sin(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
  m_ForwardDirection = glm::normalize(front);
  // also re-calculate the Right and Up vector
  m_RightDirection = glm::normalize(glm::cross(m_ForwardDirection, m_WorldUp));  // normalize the vectors, because their length gets
                                                       // closer to 0 the more you look up or down which
                                                       // results in slower movement.
  m_UpDirection = glm::normalize(glm::cross(m_RightDirection, m_ForwardDirection));
}

void Camera::AddVelocity(glm::vec2& velocity) {
  m_Position += cameraSpeed * m_ForwardDirection * velocity.y;
  m_Position += cameraSpeed * m_RightDirection * velocity.x;

  updateCameraVectors();
}

void Camera::AddRotation(glm::vec3 &rotation) {
  m_Rotation += rotation;

  updateCameraVectors();
}

const glm::vec3& Camera::getMPosition() const { return m_Position; }
void Camera::setMPosition(const glm::vec3& mPosition) { m_Position = mPosition; }
