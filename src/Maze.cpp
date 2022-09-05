//
// Created by Michael Stengel on 9/3/2022.
//

#include "Maze.h"

#include <glad/glad.h>

const float mazeUnits = 10.0f;

void drawNorthWall(std::vector<float>& walls, float mazeCenterX, float mazeCenterY) {
  // First segment
  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(0.0f);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0.0f);
  walls.push_back(0 - mazeCenterY);

  // Second segment
  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0.0f);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);
}

void drawWestWall(std::vector<float>& walls, float mazeCenterX, float mazeCenterY) {
  // First segment
  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  // Second segment
  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);
}

void drawSouthWall(std::vector<float>& walls, float mazeCenterX, float mazeCenterY) {
  // First segment
  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  // Second segment

  walls.push_back(0 - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(mazeUnits - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);
}

void drawEastWall(std::vector<float>& walls, float mazeCenterX, float mazeCenterY) {
  // First segment
  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(0);
  walls.push_back(mazeUnits - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(0);
  walls.push_back(0 - mazeCenterY);

  // Second segment
  walls.push_back(0 - mazeCenterX);
  walls.push_back(0);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(0 - mazeCenterY);

  walls.push_back(0 - mazeCenterX);
  walls.push_back(mazeUnits);
  walls.push_back(mazeUnits - mazeCenterY);
}

void drawFloor(std::vector<float>& floors, float mazeCenterX, float mazeCenterY) {
  floors.push_back(0 - mazeCenterX);
  floors.push_back(0);
  floors.push_back(0 - mazeCenterY);

  floors.push_back(0 - mazeCenterX);
  floors.push_back(0);
  floors.push_back(mazeUnits - mazeCenterY);

  floors.push_back(mazeUnits - mazeCenterX);
  floors.push_back(0);
  floors.push_back(mazeUnits - mazeCenterY);

  floors.push_back(mazeUnits - mazeCenterX);
  floors.push_back(0);
  floors.push_back(mazeUnits - mazeCenterY);

  floors.push_back(mazeUnits - mazeCenterX);
  floors.push_back(0);
  floors.push_back(0 - mazeCenterY);

  floors.push_back(0 - mazeCenterX);
  floors.push_back(0);
  floors.push_back(0 - mazeCenterY);
}

std::vector<std::shared_ptr<GameObject>> Maze::Generate(const glm::vec3& position, int width, int height) {
  std::vector<float> walls;
  std::vector<float> floors;

  unsigned int VAO1{};
  unsigned int VAO2{};

  auto mazeCenterX = (mazeUnits * static_cast<float>(width)) / 2.0f;
  auto mazeCenterY = (mazeUnits * static_cast<float>(height)) / 2.0f;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (y - 1 < 0) {
        drawNorthWall(walls, (-1 * x * mazeUnits) + mazeCenterX, (-1 * y * mazeUnits) + mazeCenterY);
      }

      if (x - 1 < 0) {
        drawEastWall(walls, (-1 * x * mazeUnits) + mazeCenterX, (-1 * y * mazeUnits) + mazeCenterY);
      }

      if (x + 1 >= width) {
        drawWestWall(walls, (-1 * x * mazeUnits) + mazeCenterX, (-1 * y * mazeUnits) + mazeCenterY);
      }

      if (y + 1 >= height) {
        drawSouthWall(walls, (-1 * x * mazeUnits) + mazeCenterX, (-1 * y * mazeUnits) + mazeCenterY);
      }

      drawFloor(floors, (-1 * x * mazeUnits) + mazeCenterX, (-1 * y * mazeUnits) + mazeCenterY);
    }
  }

  unsigned int VBO1, VBO2;
  glGenBuffers(1, &VBO1);
  glGenVertexArrays(1, &VAO1);

  glGenBuffers(1, &VBO2);
  glGenVertexArrays(1, &VAO2);

  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, walls.size() * sizeof(float), &walls.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, floors.size() * sizeof(float), &floors.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  auto wallMesh = std::make_shared<Mesh>(VAO1, walls.size() / 3);
  auto floorMesh = std::make_shared<Mesh>(VAO2, floors.size() / 3);

  auto wallMaterial = std::make_shared<Material>("default", glm::vec3(0.8f, 0.5f, 0.2f));
  auto floorMaterial = std::make_shared<Material>("default", glm::vec3(1.0f, 1.0f, 1.0f));

  std::vector<std::shared_ptr<GameObject>> objects;

  auto wallsObject = std::make_shared<GameObject>(position, wallMesh, wallMaterial);
  auto floorsObject = std::make_shared<GameObject>(position, floorMesh, floorMaterial);

  objects.push_back(wallsObject);
  objects.push_back(floorsObject);

  return objects;
}
