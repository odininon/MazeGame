//
// Created by Michael Stengel on 9/3/2022.
//

#include "Maze.h"

#include <glad/glad.h>

#include "AABB.h"

const float mazeUnits = 10.0f;

void createNorthWall(glm::vec2 position, std::vector<float>* walls, std::vector<AABB>* collisions) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x + mazeUnits);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x + mazeUnits);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x + mazeUnits);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  AABB collision({x, y - mazeUnits}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createWestWall(glm::vec2 position, std::vector<float>* walls, std::vector<AABB>* collisions) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y + mazeUnits / 2;

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y - mazeUnits);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y - mazeUnits);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y - mazeUnits);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  AABB collision({x - mazeUnits, y - mazeUnits}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createEastWall(glm::vec2 position, std::vector<float>* walls, std::vector<AABB>* collisions) {
  auto x = position.x + mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y + mazeUnits);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y + mazeUnits);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y + mazeUnits);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  AABB collision({x, y}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createSouthWall(glm::vec2 position, std::vector<float>* walls, std::vector<AABB>* collisions) {
  auto x = position.x + mazeUnits / 2;
  auto y = position.y + mazeUnits / 2;

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x - mazeUnits);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x - mazeUnits);
  walls->push_back(0);
  walls->push_back(y);

  walls->push_back(x - mazeUnits);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  walls->push_back(x);
  walls->push_back(mazeUnits);
  walls->push_back(y);

  AABB collision({x - mazeUnits, y}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createFloor(glm::vec2 position, std::vector<float>* floors, std::vector<AABB>* collisions) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  floors->push_back(x);
  floors->push_back(0);
  floors->push_back(y);

  floors->push_back(x);
  floors->push_back(0);
  floors->push_back(y + mazeUnits);

  floors->push_back(x + mazeUnits);
  floors->push_back(0);
  floors->push_back(y + mazeUnits);
}

Maze::Maze(const glm::vec3& position, int width, int height) {
  std::vector<float> walls{};
  std::vector<float> floors{0, 0, 0};

  std::vector<AABB> collisions{};

  auto mazeCenterX = position.x + width * mazeUnits / 2;
  auto mazeCenterY = position.y + height * mazeUnits / 2;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (y - 1 < 0) {
        createNorthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
      }

      if (x - 1 < 0) {
        createWestWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
      }

      if (x + 1 >= width) {
        createEastWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
      }

      if (y + 1 >= height) {
        createSouthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
      }

      createFloor({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &floors, &collisions);
    }
  }

  unsigned int VAO1{};
  unsigned int VAO2{};

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

  auto wallsObject = std::make_shared<GameObject>(position, wallMesh, wallMaterial);
  auto floorsObject = std::make_shared<GameObject>(position, floorMesh, floorMaterial);

  std::vector<std::shared_ptr<GameObject>> objects{wallsObject, floorsObject};

  m_Objects = {wallsObject, floorsObject};

  AABB mazeBounding{{position.x - mazeCenterX, position.y - mazeCenterY}, {width * mazeUnits, height * mazeUnits}};
  m_Size = mazeBounding;
  m_Collisions = collisions;
}

bool Maze::contains(glm::vec2& position) const {
  return true;
  //  return (position.x >= m_Size.m_Position.x && position.x <= m_Size.m_Position.x + m_Size.m_Size.x) &&
  //         (position.y >= m_Size.m_Position.y && position.y <= m_Size.m_Position.y + m_Size.m_Size.y);
}
