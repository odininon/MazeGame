//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <vector>

#include "GameObject.h"

class Scene {
 public:
  std::vector<GameObject *> gameObjects{};
};
