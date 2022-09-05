//
// Created by Michael Stengel on 9/5/2022.
// Copied from Breakout tutorial at learnopengl.com
//

#pragma once

#include <glad/glad.h>

#include <map>
#include <string>

#include "Shader.h"

class ResourceManager {
 public:
  static std::map<std::string, Shader> Shaders;

  static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile,
                           const std::string& name);
  static Shader GetShader(const std::string& name);

  static void Clear();

 private:
  ResourceManager() = default;
  static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
};
