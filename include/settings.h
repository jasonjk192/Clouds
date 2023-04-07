#ifndef SETTINGS_H
#define SETTINGS_H

#include <filesystem>
#include "camera.h"

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
unsigned int WINDOW_WIDTH = 1280;
unsigned int WINDOW_HEIGHT = 720;
const float aspectRatio = float(SCR_WIDTH) / float(SCR_HEIGHT);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
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

#endif //SETTINGS_H