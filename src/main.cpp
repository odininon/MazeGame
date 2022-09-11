#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "Game.h"
#include "ResourceManager.h"

#ifndef _DEBUG
#include "steam/steam_api.h"
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char* argv[]) {
#ifndef _DEBUG
  if (SteamAPI_RestartAppIfNecessary(1398840)) {
    return 1;
  }

  if (!SteamAPI_Init()) {
    return 1;
  }
#endif

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, false);

  int SCREEN_WIDTH = 1280;
  int SCREEN_HEIGHT = 720;

#ifndef _DEBUG
  GLFWmonitor* primary = glfwGetPrimaryMonitor();

  auto modes = glfwGetVideoMode(primary);
  SCREEN_WIDTH = modes->width;
  SCREEN_HEIGHT = modes->height;

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze", primary, nullptr);
#else
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze", nullptr, nullptr);
#endif

  Game* game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

  glfwMakeContextCurrent(window);
  gladLoadGL();

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  srand(time(nullptr));

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSwapInterval(1);

  game->Init(window);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    const float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();

    game->ProcessInput(deltaTime);
    game->Update(deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    game->Render();

    glfwSwapBuffers(window);
  }

  ResourceManager::Clear();

  glfwTerminate();

#ifndef _DEBUG
  SteamAPI_Shutdown();
#endif

  delete game;

  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }