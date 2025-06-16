#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

  GLFWwindow* window;

  //Screen and mouse data
  float lastX;
  float lastY;
  float fov;
  float SCR_WIDTH;
  float SCR_HEIGHT;
  bool firstMouse;

  static void zoomCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void cursorCallback(GLFWwindow* window, double xposIn, double yposIn);
public:
  Camera(float screenWidth, float screenHeight, GLFWwindow* window);

  void setupInputCallback();

  glm::mat4 getView();

  glm::mat4 getPerspective();

  void processInput(float deltaTime);
};