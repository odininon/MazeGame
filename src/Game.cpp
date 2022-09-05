//
// Created by Michael Stengel on 9/5/2022.
//

#include "Game.h"

#include "DefaultScene.h"
#include "ResourceManager.h"

Game::Game(unsigned int width, unsigned int height) : Width(width), Height(height) {}

Game::~Game() {
  delete scene;
  delete camera;
}

void Game::Init() {
  ResourceManager::LoadShader("shaders/wall.vert", "shaders/wall.frag", nullptr, "default");

  glm::vec3 cameraPosition = glm::vec3(0.0f, 5.0f, 0.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 1000.0f);

  camera = new Camera(cameraPosition, cameraFront, projection);

  scene = new DefaultScene();
}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {}

void Game::Render() {
  if (camera == nullptr || scene == nullptr) {
    return;
  }

  ResourceManager::GetShader("default").Use().SetMatrix4("projection", camera->GetViewMatrix());
  auto objects = scene->gameObjects;

  for (auto iter : objects) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    auto shader = ResourceManager::GetShader(iter->GetMaterial().GetShader()).Use();
    shader.SetMatrix4("model", glm::translate(glm::mat4(1.0f), iter->GetPosition()));
    shader.SetVector3f("color", iter->GetMaterial().GetColor());

    glBindVertexArray(iter->GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, iter->GetMesh().GetPoints());
  }
}
