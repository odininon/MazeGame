#include <iostream>

#include "GameManager.h"
#include "steam/steam_api.h"

int main() {
  if (SteamAPI_RestartAppIfNecessary(1398840))  // Replace with your App ID
  {
    return 1;
  }

  if (!SteamAPI_Init()) {
    printf(
        "Fatal Error - Steam must be running to play this game "
        "(SteamAPI_Init() failed).\n");
    return 1;
  }

  GameManager gameManager;

  try {
    gameManager.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  SteamAPI_Shutdown();

  return EXIT_SUCCESS;
}
