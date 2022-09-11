//
// Created by Michael Stengel on 9/10/2022.
//

#pragma once

#include <vector>

#include "glm/vec2.hpp"

class TextMazeGenerator {
 public:
  static void generate(std::vector<std::vector<char>>& map);
  static void showMaze(std::vector<std::vector<char>>& map);
};
