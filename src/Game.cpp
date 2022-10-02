//
// Created by Michael Stengel on 9/5/2022.
//

#include "Game.h"

#include "DefaultScene.h"
#include "ResourceManager.h"

constexpr float mouseSens = 0.25f;

Game::Game(const unsigned int width, const unsigned int height) : Width(width), Height(height) {}

Game::~Game() {}

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

  gameSetHandle = SteamInput()->GetActionSetHandle("InGameActions");

  moveHandle = SteamInput()->GetAnalogActionHandle("Move");
  cameraHandle = SteamInput()->GetAnalogActionHandle("Camera");
#endif

  toggleMouseCapture(captureMouse);
}

void Game::ProcessInput(float dt) {
#ifndef _DEBUG
  SteamInput()->RunFrame();
  auto inputHandles = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
  SteamInput()->GetConnectedControllers(inputHandles);

  inputHandle = inputHandles[0];

  SteamInput()->ActivateActionSet(inputHandle, gameSetHandle);

  InputAnalogActionData_t movementData = SteamInput()->GetAnalogActionData(inputHandle, moveHandle);
  InputAnalogActionData_t cameraData = SteamInput()->GetAnalogActionData(inputHandle, cameraHandle);
#endif

  glm::vec2 newVelocity{};

  float xoffset = 0.0f;

#ifdef _DEBUG
  ImGuiIO& io = ImGui::GetIO();
  if (!io.WantCaptureMouse && captureMouse) {
#endif

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

    GLdouble xPosIn;
    glfwGetCursorPos(m_Window, &xPosIn, nullptr);

    auto xpos = static_cast<float>(xPosIn);

    if (firstMouse) {
      lastX = xpos;

      firstMouse = false;
    }

    xoffset = xpos - lastX;
    lastX = xpos;

    xoffset *= mouseSens;

#ifdef _DEBUG
  }
#endif

#ifndef _DEBUG
  if (cameraData.bActive) {
    xoffset += cameraData.x;
  }
#endif

  playerVelocity = newVelocity;

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

  glEnable(GL_DEPTH_TEST);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  renderGameObjects();
}

void Game::renderGameObjects() const {
  ResourceManager::GetShader("default").Use().SetMatrix4("projection", camera->GetViewMatrix());
  const auto objects = scene->GetObjects();

  static glm::vec3 light_ambient = {0.2f, 0.2f, 0.2f};
  static glm::vec3 light_diffuse = {0.5f, 0.5f, 0.5f};
  static glm::vec3 light_specular = {1.0f, 1.0f, 1.0f};

  static glm::vec3 lightDirection = {-0.2f, -1.0f, 0.3f};

  ImGui::Begin("Lighting");
  ImGui::SliderFloat3("Ambient", &light_ambient[0], -1.0f, 1.0f);
  ImGui::SliderFloat3("Diffuse", &light_diffuse[0], -1.0f, 1.0f);
  ImGui::SliderFloat3("Specular", &light_specular[0], -1.0f, 1.0f);

  ImGui::SliderFloat3("Location", &lightDirection[0], -10.0f, 10.0f);
  ImGui::End();

  for (auto& iter : objects) {
    auto diffuse = iter->GetMaterial().GetData().Diffuse;
    auto specular = iter->GetMaterial().GetData().Specular;
    auto shininess = iter->GetMaterial().GetData().Shininess;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Shader shader = ResourceManager::GetShader(iter->GetMaterial().GetShader()).Use();
    shader.SetMatrix4("model", glm::translate(glm::mat4(1.0f), iter->GetPosition()));

    shader.SetVector3f("color", iter->GetMaterial().GetData().ObjectColor);

    shader.SetVector3f("viewPos", camera->GetPosition());

    shader.SetFloat("material.diffuse", diffuse);
    shader.SetFloat("material.specular", specular);
    shader.SetFloat("material.shininess", shininess);

    shader.SetVector3f("light.direction", lightDirection);
    shader.SetVector3f("light.ambient", light_ambient);
    shader.SetVector3f("light.diffuse", light_diffuse);
    shader.SetVector3f("light.specular", light_specular);

    glBindVertexArray(iter->GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, iter->GetMesh().GetPoints());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

bool Game::checkWallCollision(const glm::vec3& player) const {
  AABB playerAABB({player.x - (2 / 2.0f), player.z - (2 / 2.0f)}, {2, 2});

  return scene->CheckCollisions(playerAABB);
}

void Game::processKey(GLFWwindow* window, int key, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
#ifdef _DEBUG
    toggleMouseCapture(!captureMouse);
#else
    glfwSetWindowShouldClose(window, true);
#endif
  }
}

void Game::toggleMouseCapture(bool b) {
  captureMouse = b;
  if (b) {
    glfwSetInputMode(this->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(this->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}
