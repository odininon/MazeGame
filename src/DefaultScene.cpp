//
// Created by Michael Stengel on 9/5/2022.
//

#include "DefaultScene.h"
DefaultScene::DefaultScene() {
  auto maze = std::make_shared<Maze>(glm::vec3(0, 0, 0), 11, 11);

  m_Mazes.push_back(maze);
}
