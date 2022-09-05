//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include <vector>
#include <memory>

#include "GameObject.h"

class Scene {
 public:
  std::vector<std::shared_ptr<GameObject>> m_GameObjects{};
};
