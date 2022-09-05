//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <memory>
#include <utility>

#include "Material.h"
#include "Mesh.h"
class GameObject {
 public:
  explicit GameObject(glm::vec3 position, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
      : Position(position), Mesh(std::move(mesh)), Material(std::move(material)) {}

  ~GameObject() = default;

  glm::vec3 GetPosition() { return Position; }
  Mesh GetMesh() { return *Mesh; }
  Material GetMaterial() { return *Material; }

 private:
  glm::vec3 Position;

  std::shared_ptr<Mesh> Mesh;
  std::shared_ptr<Material> Material;
};
