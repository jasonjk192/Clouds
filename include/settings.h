#ifndef SETTINGS_H
#define SETTINGS_H

#include <filesystem>
#include "camera.h"

// settings
unsigned int WINDOW_WIDTH = 1280;
unsigned int WINDOW_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = camera.width / 2.0f;
float lastY = camera.height / 2.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightCol(1.f, 1.f, 1.f);

// paths
std::filesystem::path shaderPath = std::filesystem::current_path().append("shaders");

// shaders
std::filesystem::path flatVertPath = absolute(shaderPath).append("flat.vert");
std::filesystem::path flatFragPath = absolute(shaderPath).append("flat.frag");

std::filesystem::path screenVertPath = absolute(shaderPath).append("screen.vert");
std::filesystem::path screenFragPath = absolute(shaderPath).append("screen.frag");

std::filesystem::path depthVertPath = absolute(shaderPath).append("depth.vert");
std::filesystem::path depthFragPath = absolute(shaderPath).append("depth.frag");

std::filesystem::path raymarchVertPath = absolute(shaderPath).append("raymarch.vert");
std::filesystem::path raymarchFragPath = absolute(shaderPath).append("raymarch.frag");

std::filesystem::path fogVertPath = absolute(shaderPath).append("fog.vert");
std::filesystem::path fogFragPath = absolute(shaderPath).append("fog.frag");

std::filesystem::path noiseVertPath = absolute(shaderPath).append("fBm.vert");
std::filesystem::path noiseFragPath = absolute(shaderPath).append("fBm.frag");

std::filesystem::path cloudVertPath = absolute(shaderPath).append("cloud.vert");
std::filesystem::path cloudFragPath = absolute(shaderPath).append("cloud.frag");

// other assets
std::filesystem::path valueNoiseImagePath = std::filesystem::current_path().append("assets/ValueNoise.png");

#endif //SETTINGS_H