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
  float mPitch;
  float mYaw;

  // Camera Attributes
  glm::vec3 mCameraPos;
  glm::vec3 mCameraUp;
  glm::vec3 mCameraFront;
  glm::vec3 mCameraRight;
  const glm::vec3 mWorldUp;

  GLFWwindow* mWindow;

  //Screen and mouse data
  float mLastX;
  float mLastY;
  float mFov;
  bool mFirstMouse;

  static void zoomCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void cursorCallback(GLFWwindow* window, double xposIn, double yposIn);
  void updateCameraVectors();
public:
  Camera(GLFWwindow* window);

  void setupInputCallback();

  glm::mat4 getView() { return glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp); }

  glm::mat4 getPerspective(float screenWidth, float screenHeight);

  float getFov() { return mFov; }

  glm::vec3 getCameraPos() { return mCameraPos; }
  glm::vec3 getCameraFront() { return mCameraFront; }

  void processInput(float deltaTime);
};