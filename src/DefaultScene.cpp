//
// Created by Michael Stengel on 9/5/2022.
//

#include "DefaultScene.h"
DefaultScene::DefaultScene() {
  const auto maze = std::make_shared<Maze>(glm::vec3(0, 0, 0), 101, 101);

  m_mazes_.push_back(maze);
}

glm::vec3 DefaultScene::defaultCameraPosition() const {

  return m_mazes_[0]->StartingPosition();
}
