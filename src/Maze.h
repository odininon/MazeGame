//
// Created by Michael Stengel on 9/3/2022.
//

#pragma once

#include <memory>
#include <vector>

#include "AABB.h"
#include "GameObject.h"
#include "ResourceManager.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
};

class Maze {
 public:
  explicit Maze(const glm::vec3& position, int width, int height);

  const std::vector<std::shared_ptr<GameObject>>& GetObjects() { return m_Objects; }
  const std::vector<AABB>& GetCollisions() const { return m_Collisions; }

  bool contains(glm::vec2& position) const;

  bool CheckCollisions(AABB& other) {
    for (auto& itr : m_Collisions) {
      if (itr.intersects(other)) {
        return true;
      }
    }

    return false;
  }

  glm::vec3 StartingPosition() const { return m_startingPosition_; }

 private:
  std::vector<std::shared_ptr<GameObject>> m_Objects;
  std::vector<AABB> m_Collisions{};
  glm::vec3 m_startingPosition_;
  AABB m_Size;
};
