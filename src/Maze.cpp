//
// Created by Michael Stengel on 9/3/2022.
//

#include "Maze.h"

#include <glad/glad.h>

#include <iostream>

const float mazeUnits = 10.0f;

const char* fillVertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fillFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(0.8f, 0.5f, 0.2f, 0.0f);\n"
    "} ";

const char* lineVertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* lineFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);\n"
    "} ";

void drawNorthWall(std::vector<float>& walls, float mazeCenterX,
                   float mazeCenterY) {
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

void drawEastWall(std::vector<float>& walls, float mazeCenterX,
                  float mazeCenterY) {
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

void drawFloor(std::vector<float>& floors, float mazeCenterX,
               float mazeCenterY) {
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

unsigned int createShaderProgram(const char* vertexShaderSource,
                                 const char* fragmentShaderSource) {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

Maze::Maze(int width, int height) {
  auto mazeCenterX = (mazeUnits * static_cast<float>(width)) / 2.0f;
  auto mazeCenterY = (mazeUnits * static_cast<float>(height)) / 2.0f;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (y - 1 < 0) {
        drawNorthWall(walls, (-1 * x * mazeUnits) + mazeCenterX,
                      (-1 * y * mazeUnits) + mazeCenterY);
      }

      if (x - 1 < 0) {
        drawEastWall(walls, (-1 * x * mazeUnits) + mazeCenterX,
                     (-1 * y * mazeUnits) + mazeCenterY);
      }

      drawFloor(floors, (-1 * x * mazeUnits) + mazeCenterX,
                (-1 * y * mazeUnits) + mazeCenterY);
    }
  }

  glGenBuffers(1, &VBO1);
  glGenVertexArrays(1, &VAO1);

  glGenBuffers(1, &VBO2);
  glGenVertexArrays(1, &VAO2);

  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, walls.size() * sizeof(float), &walls.front(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void*)nullptr);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, floors.size() * sizeof(float), &floors.front(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void*)nullptr);
  glEnableVertexAttribArray(0);

  glm::mat4 Projection = glm::perspective(
      glm::radians(45.0f), (float)800 / (float)600, 0.1f, 1000.0f);

  glm::vec3 position = glm::vec3(10, 5, 10);

  // Camera matrix
  glm::mat4 View =
      glm::lookAt(position, position + glm::vec3(sqrt(2), 0, -sqrt(2)),
                  glm::vec3(0, 1, 0));

  glm::mat4 Model = glm::mat4(1.0f);

  mvp = Projection * View * Model;

  fillShader =
      createShaderProgram(fillVertexShaderSource, fillFragmentShaderSource);

  lineShader =
      createShaderProgram(lineVertexShaderSource, lineFragmentShaderSource);
}

void Maze::draw() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glUseProgram(fillShader);

  auto MatrixID = glGetUniformLocation(fillShader, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(VAO1);
  glDrawArrays(GL_TRIANGLES, 0, walls.size() / 3);

  glUseProgram(lineShader);

  glBindVertexArray(VAO2);
  glDrawArrays(GL_TRIANGLES, 0, floors.size() / 3);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glUseProgram(lineShader);

  MatrixID = glGetUniformLocation(lineShader, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(VAO1);
  glDrawArrays(GL_TRIANGLES, 0, walls.size() / 3);

  glUseProgram(fillShader);

  glBindVertexArray(VAO2);
  glDrawArrays(GL_TRIANGLES, 0, floors.size() / 3);
}
