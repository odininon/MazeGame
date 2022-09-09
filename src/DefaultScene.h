//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

#include "Maze.h"
#include "Scene.h"

class DefaultScene : public Scene {
 public:
  DefaultScene();

  std::vector<std::shared_ptr<GameObject>> GetObjects() {
    std::vector<std::shared_ptr<GameObject>> objects;

    for (const auto& iter : m_Mazes) {
      for (const auto& objs : iter->GetObjects()) {
        objects.push_back(objs);
      }
    }

    return objects;
  }

  void DrawCollisions() {
    for (const auto& iter : m_Mazes) {
      for (auto objs : iter->GetCollisions()) {
        objs.Draw();
      }
    }
  }

  bool CheckCollisions(AABB& player) {
    for (const auto& iter : m_Mazes) {
      if (iter->contains(player.m_Position)) {
        return iter->CheckCollisions(player);
      }
    }

    return false;
  }

 private:
  std::vector<std::shared_ptr<Maze>> m_Mazes{};
};
