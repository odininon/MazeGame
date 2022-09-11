//
// Created by Michael Stengel on 9/5/2022.
//

#include "Game.h"

#include "DefaultScene.h"
#include "ResourceManager.h"

constexpr float mouseSens = 0.25f;

Game::Game(const unsigned int width, const unsigned int height) : Width(width), Height(height) {}

Game::~Game() = default;

void Game::Init(GLFWwindow* window) {
  m_Window = window;
  ResourceManager::LoadShader("shaders/wall.vert", "shaders/wall.frag", nullptr, "default");

  auto cameraPosition = glm::vec3(0.0f, 5.0f, 0.0f);
  auto cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 1000.0f);

  scene = std::make_unique<DefaultScene>();

  glm::vec3 startingPos = scene->defaultCameraPosition();
  camera = std::make_unique<Camera>(startingPos, cameraFront, projection);

#ifndef _DEBUG
  SteamInput()->Init(false);

  SteamInput()->RunFrame();

  auto inputHandles = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
  SteamInput()->GetConnectedControllers(inputHandles);

  inputHandle = inputHandles[0];
  gameSetHandle = SteamInput()->GetActionSetHandle("InGameActions");

  moveHandle = SteamInput()->GetAnalogActionHandle("Move");
  cameraHandle = SteamInput()->GetAnalogActionHandle("Camera");
#endif
}

void Game::ProcessInput(float dt) {
#ifndef _DEBUG
  SteamInput()->RunFrame();

  SteamInput()->ActivateActionSet(inputHandle, gameSetHandle);

  InputAnalogActionData_t movementData = SteamInput()->GetAnalogActionData(inputHandle, moveHandle);

  InputAnalogActionData_t cameraData = SteamInput()->GetAnalogActionData(inputHandle, cameraHandle);
#endif

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

#ifndef _DEBUG
  if (movementData.bActive) {
    newVelocity += glm::vec2(movementData.x, movementData.y);
  }
#endif

  if (length(newVelocity) > 1) {
    newVelocity = normalize(newVelocity);
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

#ifndef _DEBUG
  if (cameraData.bActive) {
    xoffset += cameraData.x;
  }
#endif

  xoffset *= mouseSens;

  playerRotation = glm::vec3(xoffset, 0.0f, 0.0f);
}

void Game::Update(const float dt) {
  const glm::vec3 startingPosition = camera->getMPosition();
  auto velocity = playerVelocity * dt;

  camera->AddVelocity(velocity);
  camera->AddRotation(playerRotation);

  const glm::vec3 endingPosition = camera->getMPosition();

  if (checkWallCollision(endingPosition)) {
    camera->setMPosition(startingPosition);
  }
}

void Game::Render() {
  if (camera == nullptr || scene == nullptr) {
    return;
  }

  ResourceManager::GetShader("default").Use().SetMatrix4("projection", camera->GetViewMatrix());
  const auto objects = scene->GetObjects();

  for (const auto& iter : objects) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Shader shader = ResourceManager::GetShader(iter->GetMaterial().GetShader()).Use();
    shader.SetMatrix4("model", glm::translate(glm::mat4(1.0f), iter->GetPosition()));
    shader.SetVector3f("color", iter->GetMaterial().GetColor());

    glBindVertexArray(iter->GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, iter->GetMesh().GetPoints());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

bool Game::checkWallCollision(const glm::vec3& player) const {
  AABB playerAABB({player.x - (2 / 2.0f), player.z - (2 / 2.0f)}, {2, 2});

  return scene->CheckCollisions(playerAABB);
}
