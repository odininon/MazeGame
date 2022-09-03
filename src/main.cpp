#include <iostream>

#include "GameManager.h"

int main() {
  GameManager gameManager;

  try {
    gameManager.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
