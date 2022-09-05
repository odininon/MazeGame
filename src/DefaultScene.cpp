//
// Created by Michael Stengel on 9/5/2022.
//

#include "DefaultScene.h"
DefaultScene::DefaultScene() {
  auto mazeObjects = Maze::Generate(glm::vec3(0, 0, 0), 21, 21);

  for (auto iter : mazeObjects) {
    gameObjects.push_back(iter);
  }
}