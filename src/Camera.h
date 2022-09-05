//
// Created by Michael Stengel on 9/4/2022.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera {
 public:
  explicit Camera(glm::vec3 &position, glm::vec3 &forward, glm::mat4 &projection)
      : Position(position), ForwardDirection(forward), Projection(projection) {
    View = glm::lookAt(Position, Position + ForwardDirection, UpDirection);
  }

  glm::mat4 GetViewMatrix() { return Projection * View; }

 private:
  glm::mat4 View{};
  glm::mat4 Projection;

  glm::vec3 UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 ForwardDirection;
  glm::vec3 Position;
};
