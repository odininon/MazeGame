//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "ResourceManager.h"

class Maze {
 public:
  static std::vector<std::shared_ptr<GameObject>> Generate(const glm::vec3& position, int width, int height);
};
