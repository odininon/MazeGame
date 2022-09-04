//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.h"
class Maze {
 public:
  explicit Maze(int width, int height);
  ~Maze() = default;

  void draw(Camera* camera);

 private:
  std::vector<float> walls;
  std::vector<float> floors;

  unsigned int VAO1{};
  unsigned int VBO1{};

  unsigned int VAO2{};
  unsigned int VBO2{};

  unsigned int fillShader;
  unsigned int lineShader;

  glm::mat4 Model;
};
