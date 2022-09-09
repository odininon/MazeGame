//
// Created by Michael Stengel on 9/5/2022.
//

#include "Game.h"

#include <iostream>

#include "DefaultScene.h"
#include "ResourceManager.h"

const float mouseSens = 0.25f;

Game::Game(unsigned int width, unsigned int height) : Width(width), Height(height) {}

Game::~Game() = default;

void Game::Init(GLFWwindow* window) {
  m_Window = window;
  ResourceManager::LoadShader("shaders/wall.vert", "shaders/wall.frag", nullptr, "default");

  glm::vec3 cameraPosition = glm::vec3(0.0f, 5.0f, 0.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 1000.0f);

  camera = std::make_unique<Camera>(cameraPosition, cameraFront, projection);
  scene = std::make_unique<DefaultScene>();
}

void Game::ProcessInput(float dt) {
  glm::vec2 newVelocity{};

  if (glfwGetKey(m_Window, GLFW_KEY_W)) {
    newVelocity.y += 1;
  }

  if (glfwGetKey(m_Window, GLFW_KEY_S)) {
    newVelocity.y -= 1;
  }

  if (glfwGetKey(m_Window, GLFW_KEY_A)) {
    newVelocity.x -= 1;
  }

  if (glfwGetKey(m_Window, GLFW_KEY_D)) {
    newVelocity.x += 1;
  }

  if (glm::length(newVelocity) > 1) {
    newVelocity = glm::normalize(newVelocity);
  }

  playerVelocity = newVelocity;

  GLdouble xPosIn;
  glfwGetCursorPos(m_Window, &xPosIn, nullptr);

  auto xpos = static_cast<float>(xPosIn);

  if (firstMouse) {
    lastX = xpos;

    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  lastX = xpos;

  xoffset *= mouseSens;

  playerRotation = glm::vec3(xoffset, 0.0f, 0.0f);
}

void Game::Update(float dt) {
  auto startingPosition = camera->getMPosition();
  auto velocity = playerVelocity * dt;

  camera->AddVelocity(velocity);
  camera->AddRotation(playerRotation);

  auto endingPosition = camera->getMPosition();

  if (checkWallCollision(endingPosition)) {
    camera->setMPosition(startingPosition);
  }
}

void Game::Render() {
  if (camera == nullptr || scene == nullptr) {
    return;
  }

  ResourceManager::GetShader("default").Use().SetMatrix4("projection", camera->GetViewMatrix());
  auto objects = scene->GetObjects();

  for (const auto& iter : objects) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    auto shader = ResourceManager::GetShader(iter->GetMaterial().GetShader()).Use();
    shader.SetMatrix4("model", glm::translate(glm::mat4(1.0f), iter->GetPosition()));
    shader.SetVector3f("color", iter->GetMaterial().GetColor());

    glBindVertexArray(iter->GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, iter->GetMesh().GetPoints());
  }
}

bool Game::checkWallCollision(glm::vec3& player) {
  AABB playerAABB({player.x - (5 / 2.0f), player.z - (5 / 2.0f)}, {5, 5});

  return scene->CheckCollisions(playerAABB);
}
