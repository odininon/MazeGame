//
// Created by Michael Stengel on 9/10/2022.
//

#include "TextMazeGenerator.h"

#include <iostream>

void swap(int& a, int& b) {
  int c = a;
  a = b;
  b = c;
}

int countVisitedNeighbor(std::vector<std::vector<char>>& map, int i, int j) {
  int direct[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  int count = 0;
  for (auto & k : direct) {
    int ni = i + k[0];
    int nj = j + k[1];
    // out of boundary
    if (ni < 0 || nj < 0 || ni >= map.size() || nj >= map[0].size()) continue;
    if (map[ni][nj] == ' ') count++;  // visited
  }
  return count;
}

void shuffle(int a[], int n) {
  for (int i = 0; i < n; ++i) {
    swap(a[i], a[rand() % n]);
  }
}

void doGenerate(std::vector<std::vector<char>>& map, int i, int j) {
  int direct[][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
  int visitOrder[] = {0, 1, 2, 3};
  // out of boundary
  if (i < 0 || j < 0 || i >= map.size() || j >= map[0].size()) return;
  // visited, go back to the coming direction, return
  if (map[i][j] == ' ') return;

  // some neightbors are visited in addition to the coming direction, return
  // this is to avoid circles in maze
  if (countVisitedNeighbor(map, i, j) > 1) return;

  map[i][j] = ' ';  // visited

  // shuffle the visitOrder
  shuffle(visitOrder, 4);

  for (int k : visitOrder) {
    int ni = i + direct[k][0];
    int nj = j + direct[k][1];
    doGenerate(map, ni, nj);
  }
}

void TextMazeGenerator::generate(std::vector<std::vector<char>>& map) {
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[0].size(); j++) {
      map[i][j] = '*';
    }
  }
  doGenerate(map,0,0);
}

void TextMazeGenerator::showMaze(std::vector<std::vector<char>>& map) {
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[0].size(); j++) {
      std::cout << map[i][j];
    }
    std::cout << std::endl;
  }
}
