//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include "Maze.h"
#include "Scene.h"

#include <memory>

class DefaultScene : public Scene {
 public:
  DefaultScene();

  std::vector<std::shared_ptr<GameObject>> GetObjects() const {
    std::vector<std::shared_ptr<GameObject>> objects;

    for (const auto& iter : m_mazes_) {
      for (const auto& objs : iter->GetObjects()) {
        objects.push_back(objs);
      }
    }

    return objects;
  }

  bool CheckCollisions(AABB& player) const {
    for (const auto& iter : m_mazes_) {
      if (iter->contains(player.m_position)) {
        return iter->CheckCollisions(player);
      }
    }

    return false;
  }

  glm::vec3 defaultCameraPosition() const;

private:
  std::vector<std::shared_ptr<Maze>> m_mazes_{};
};
