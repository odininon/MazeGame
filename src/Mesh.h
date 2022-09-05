//
// Created by Michael Stengel on 9/5/2022.
//

#pragma once

class Mesh {
 public:
  explicit Mesh(unsigned int vertexArray, int points) : VAO(vertexArray), Points(points) {}

  unsigned int GetVAO() { return VAO; }
  int GetPoints() { return Points; }

 private:
  unsigned int VAO{};
  int Points;
};
