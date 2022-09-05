//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include "Material.h"
#include "Mesh.h"
class GameObject {
 public:
  explicit GameObject(glm::vec3 position, Mesh *mesh, Material *material)
      : Position(position), Mesh(mesh), Material(material) {}

  ~GameObject() {
    delete Mesh;
    delete Material;
  }

  glm::vec3 GetPosition() { return Position; }
  Mesh GetMesh() { return *Mesh; }
  Material GetMaterial() { return *Material; }

 private:
  glm::vec3 Position;

  Mesh *Mesh;
  Material *Material;
};
