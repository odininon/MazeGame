//
// Created by Michael Stengel on 9/3/2022.
//

#include "Maze.h"

#include <glad/glad.h>

#include "AABB.h"
#include "TextMazeGenerator.h"

const float mazeUnits = 10.0f;

void createNorthWall(glm::vec2 position, std::vector<Vertex>* walls, std::vector<AABB>* collisions) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  glm::vec3 normal = {0, 0, 1};

  walls->push_back(Vertex{{x, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, 0, y}, normal});
  walls->push_back(Vertex{{x + mazeUnits, 0, y}, normal});

  walls->push_back(Vertex{{x + mazeUnits, 0, y}, normal});
  walls->push_back(Vertex{{x + mazeUnits, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y}, normal});

  AABB collision({x, y - mazeUnits}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createWestWall(glm::vec2 position, std::vector<Vertex>* walls, std::vector<AABB>* collisions) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y + mazeUnits / 2;

  glm::vec3 normal = {1, 0, 0};

  walls->push_back(Vertex{{x, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, 0, y}, normal});
  walls->push_back(Vertex{{x, 0, y - mazeUnits}, normal});

  walls->push_back(Vertex{{x, 0, y - mazeUnits}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y - mazeUnits}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y}, normal});

  AABB collision({x - mazeUnits, y - mazeUnits}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createEastWall(glm::vec2 position, std::vector<Vertex>* walls, std::vector<AABB>* collisions) {
  auto x = position.x + mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  glm::vec3 normal = {-1, 0, 0};

  walls->push_back(Vertex{{x, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, 0, y}, normal});
  walls->push_back(Vertex{{x, 0, y + mazeUnits}, normal});

  walls->push_back(Vertex{{x, 0, y + mazeUnits}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y + mazeUnits}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y}, normal});

  AABB collision({x, y}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createSouthWall(glm::vec2 position, std::vector<Vertex>* walls, std::vector<AABB>* collisions) {
  auto x = position.x + mazeUnits / 2;
  auto y = position.y + mazeUnits / 2;

  glm::vec3 normal = {0, 0, -1};

  walls->push_back(Vertex{{x, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, 0, y}, normal});
  walls->push_back(Vertex{{x - mazeUnits, 0, y}, normal});

  walls->push_back(Vertex{{x - mazeUnits, 0, y}, normal});
  walls->push_back(Vertex{{x - mazeUnits, mazeUnits, y}, normal});
  walls->push_back(Vertex{{x, mazeUnits, y}, normal});

  AABB collision({x - mazeUnits, y}, {mazeUnits, mazeUnits});
  collisions->push_back(collision);
}

void createFloor(glm::vec2 position, std::vector<Vertex>* floors) {
  auto x = position.x - mazeUnits / 2;
  auto y = position.y - mazeUnits / 2;

  glm::vec3 normal = {0, 1, 0};

  floors->push_back(Vertex{{x, 0, y}, normal});
  floors->push_back(Vertex{{x, 0, y + mazeUnits}, normal});
  floors->push_back(Vertex{{x + mazeUnits, 0, y + mazeUnits}, normal});

  floors->push_back(Vertex{{x + mazeUnits, 0, y + mazeUnits}, normal});
  floors->push_back(Vertex{{x + mazeUnits, 0, y}, normal});
  floors->push_back(Vertex{{x, 0, y}, normal});
}

Maze::Maze(const glm::vec3& position, int width, int height) {
  std::vector<std::vector<char>> textMaze;
  std::vector<char> row(width + 1);

  textMaze.reserve(height + 1);
  for (int i = 0; i <= height; i++) {
    textMaze.push_back(row);
  }

  TextMazeGenerator::generate(textMaze);

  std::vector<Vertex> walls{};
  std::vector<Vertex> floors{};

  std::vector<AABB> collisions{};

  auto mazeCenterX = position.x + width * mazeUnits / 2;
  auto mazeCenterY = position.y + height * mazeUnits / 2;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (textMaze[y][x] != '*') {
        if (y - 1 < 0) {
          createNorthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
        }

        if (y + 1 >= height) {
          createSouthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
        }

        if (x - 1 < 0) {
          createWestWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
        }

        if (x + 1 >= width) {
          createEastWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
        }

        auto northNeighborY = y - 1;
        if (northNeighborY >= 0) {
          if (textMaze[northNeighborY][x] == '*') {
            createNorthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
          }
        }

        northNeighborY = y + 1;
        if (northNeighborY < height) {
          if (textMaze[northNeighborY][x] == '*') {
            createSouthWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
          }
        }

        auto northNeighborX = x - 1;
        if (northNeighborX >= 0) {
          if (textMaze[y][northNeighborX] == '*') {
            createWestWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
          }
        }

        northNeighborX = x + 1;
        if (northNeighborX < width) {
          if (textMaze[y][northNeighborX] == '*') {
            createEastWall({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &walls, &collisions);
          }
        }

        createFloor({x * mazeUnits - mazeCenterX, y * mazeUnits - mazeCenterY}, &floors);
      }
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
  glBufferData(GL_ARRAY_BUFFER, walls.size() * sizeof(Vertex), &walls.front(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glBindVertexArray(0);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, floors.size() * sizeof(Vertex), &floors.front(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glBindVertexArray(0);

  auto wallMesh = std::make_shared<Mesh>(VAO1, walls.size() / 3);
  auto floorMesh = std::make_shared<Mesh>(VAO2, floors.size() / 3);

  MaterialData wallMaterialData{{0.8f, 0.5f, 0.2f}, 1, 1, 1};
  auto wallMaterial = std::make_shared<Material>("default", wallMaterialData);

  MaterialData floorMaterialData{{0.8f, 0.5f, 0.2f}, 1, 1, 1};
  auto floorMaterial = std::make_shared<Material>("default", floorMaterialData);

  auto wallsObject = std::make_shared<GameObject>(position, wallMesh, wallMaterial);
  auto floorsObject = std::make_shared<GameObject>(position, floorMesh, floorMaterial);

  std::vector<std::shared_ptr<GameObject>> objects{wallsObject, floorsObject};

  m_Objects = {wallsObject, floorsObject};

  AABB mazeBounding{{position.x - mazeCenterX, position.y - mazeCenterY}, {width * mazeUnits, height * mazeUnits}};
  m_Size = mazeBounding;
  m_Collisions = collisions;

  m_startingPosition_ = glm::vec3(-mazeCenterX, 5, -mazeCenterY);
}

bool Maze::contains(glm::vec2& position) const {
  return true;
  //  return (position.x >= m_Size.m_Position.x && position.x <= m_Size.m_Position.x + m_Size.m_Size.x) &&
  //         (position.y >= m_Size.m_Position.y && position.y <= m_Size.m_Position.y + m_Size.m_Size.y);
}
