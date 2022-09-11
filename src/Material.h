//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <utility>

struct MaterialData {
  glm::vec3 ObjectColor;
  float Diffuse;
  float Specular;
  float Shininess;
};

class Material {
 public:
  explicit Material(std::string shader, MaterialData data) : m_Shader(std::move(shader)), m_Data(data){};

  std::string GetShader() const { return m_Shader; }
  MaterialData GetData() const { return m_Data; }

 private:
  std::string m_Shader;
  MaterialData m_Data;
};
