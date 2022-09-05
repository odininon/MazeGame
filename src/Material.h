//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <utility>

class Material {
 public:
  explicit Material(std::string shader, glm::vec3 color) : m_Shader(std::move(shader)), m_Color(color){};

  std::string GetShader() { return m_Shader; }
  glm::vec3 GetColor() { return m_Color; }

 private:
  std::string m_Shader;
  glm::vec3 m_Color;
};
