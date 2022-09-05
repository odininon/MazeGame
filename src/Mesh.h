//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

class Mesh {
 public:
  explicit Mesh(unsigned int vertexArray, int points) : m_VAO(vertexArray), m_Points(points) {}

  unsigned int GetVAO() { return m_VAO; }
  int GetPoints() { return m_Points; }

 private:
  unsigned int m_VAO{};
  int m_Points;
};
