//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <utility>

class Material {
 public:
  explicit Material(std::string shader, glm::vec3 color) : Shader(std::move(shader)), Color(color){};

  std::string GetShader() { return Shader; }
  glm::vec3 GetColor() { return Color; }

 private:
  std::string Shader;
  glm::vec3 Color;
};
