#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera
{
private:
  //Euler angles
  float pitch;
  float yaw;

  // Camera Attributes
  glm::vec3 cameraPos;
  glm::vec3 cameraUp;
  glm::vec3 cameraFront;
  glm::vec3 cameraRight;
  const glm::vec3 worldUp;

  GLFWwindow* window;

  //Screen and mouse data
  float lastX;
  float lastY;
  float fov;
  bool firstMouse;

  static void zoomCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void cursorCallback(GLFWwindow* window, double xposIn, double yposIn);
  void updateCameraVectors();
public:
  Camera(GLFWwindow* window);

  void setupInputCallback();

  glm::mat4 getView();

  glm::mat4 getPerspective(float screenWidth, float screenHeight);

  float getFov();

  glm::vec3 getCameraPos();

  void processInput(float deltaTime);
};