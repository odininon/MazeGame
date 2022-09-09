//
// Created by Michael Stengel on 9/7/2022.
//

#ifndef MAZEGAMECONAN_AABB_H
#define MAZEGAMECONAN_AABB_H

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <vector>

#include "ResourceManager.h"
class AABB {
 public:
  AABB() = default;

  AABB(glm::vec2 position, glm::vec2 size) : m_Position(position), m_Size(size) {
    Points.push_back(0);
    Points.push_back(10);
    Points.push_back(0);

    Points.push_back(0);
    Points.push_back(0);
    Points.push_back(0);

    Points.push_back(0);
    Points.push_back(0);
    Points.push_back(0 + size.y);

    Points.push_back(0);
    Points.push_back(0);
    Points.push_back(0 + size.y);

    Points.push_back(0);
    Points.push_back(10);
    Points.push_back(0 + size.y);

    Points.push_back(0);
    Points.push_back(10);
    Points.push_back(0);

    Points.push_back(size.x);
    Points.push_back(10);
    Points.push_back(size.y);

    Points.push_back(size.x);
    Points.push_back(0);
    Points.push_back(size.y);

    Points.push_back(size.y);
    Points.push_back(0);
    Points.push_back(0);

    Points.push_back(size.y);
    Points.push_back(0);
    Points.push_back(0);

    Points.push_back(size.y);
    Points.push_back(10);
    Points.push_back(0);

    Points.push_back(size.x);
    Points.push_back(10);
    Points.push_back(size.y);

    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(float), &Points.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
  }

  bool intersects(AABB& first);

  void Draw() {
    auto shader = ResourceManager::GetShader("default").Use();
    shader.SetMatrix4("model", glm::translate(glm::mat4(1.0f), glm::vec3(m_Position.x, 0, m_Position.y)));
    shader.SetVector3f("color", 0.0, 0.0, 1.0);

    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, Points.size());
  }

 public:
  glm::vec2 m_Position{};
  glm::vec2 m_Size{};

 private:
  std::vector<float> Points{};
  unsigned int VAO1{};
};

#endif  // MAZEGAMECONAN_AABB_H
