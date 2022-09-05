#include <iostream>

#include "GameManager.h"

#ifdef ENABLE_STEAM
#include "steam/steam_api.h"
#endif

int main() {
#ifdef ENABLE_STEAM
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
#endif

  GameManager gameManager;

  try {
    gameManager.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

#ifdef ENABLE_STEAM
  SteamAPI_Shutdown();
#endif

  return EXIT_SUCCESS;
}
