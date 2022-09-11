//
// Created by Michael Stengel on 9/7/2022.
//

#pragma once

#include <glm/vec2.hpp>
#include "ResourceManager.h"

class AABB {
public:
  AABB() = default;

  AABB(const glm::vec2 position, const glm::vec2 size)
    : m_position(position),
      m_size(size) {
  }

  bool intersects(const AABB& first) const;

public:
  glm::vec2 m_position{};
  glm::vec2 m_size{};
};
