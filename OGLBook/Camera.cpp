#include "Camera.h"

void Camera::zoomCallback(GLFWwindow* mainWindow, double xoffset, double yoffset)
{
  Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(mainWindow));
  if (!camera) {
    std::cout << "ERROR IN RETRIEVING CAMERA FOR ZOOM CALLBACK" << std::endl;
    return;
  }
  camera->mFov -= static_cast<float>(yoffset);

  if (camera->mFov < 1.f)
    camera->mFov = 1.f;
  if (camera->mFov > 60.f)
    camera->mFov = 60.f;
}

void Camera::cursorCallback(GLFWwindow* mainWindow, double xposIn, double yposIn)
{
  Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(mainWindow));
  if (!camera) {
    std::cout << "ERROR IN RETRIEVING CAMERA FOR CURSOR CALLBACK" << std::endl;
    return;
  }
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (camera->mFirstMouse) {
    camera->mLastX = xpos;
    camera->mLastY = ypos;
    camera->mFirstMouse = false;
  }

  float xOffSet = xpos - camera->mLastX;
  float yOffSet = camera->mLastY - ypos; //Reverse because y-coord go from bottom to top;

  camera->mLastX = xpos;
  camera->mLastY = ypos;

  const float sensitivity = 0.1f;
  xOffSet *= sensitivity;
  yOffSet *= sensitivity;

  camera->mYaw += xOffSet;
  camera->mPitch += yOffSet;

  if (camera->mPitch > 89.f)
    camera->mPitch = 89.f;
  if (camera->mPitch < -89.f)
    camera->mPitch = -89.f;

  camera->updateCameraVectors();
}

Camera::Camera(GLFWwindow* mainWindow) : 
  mWindow(mainWindow), 
  mPitch(0.f),
  mYaw(-90.f), 
  mCameraFront(glm::vec3(0.f, 0.f, -1.f)), 
  mCameraPos(glm::vec3(0.0f, 0.0f, 3.0f)), 
  mCameraUp(glm::vec3(0.f, 1.f, 0.f)), 
  mFirstMouse(true), 
  mFov(45.f), 
  mWorldUp(glm::vec3(0.f, 1.f, 0.f))
{
  int screenWidth, screenHeight;

  glfwGetFramebufferSize(mainWindow, &screenWidth, &screenHeight);

  mLastX = static_cast<float>(screenWidth) / 2.f;
  mLastY = static_cast<float>(screenHeight) / 2.f;

  updateCameraVectors();
} 

void Camera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  front.y = sin(glm::radians(mPitch));
  front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mCameraFront = glm::normalize(front);

  // Recalculate Right and Up vectors
  mCameraRight = glm::normalize(glm::cross(mCameraFront, mWorldUp));
  mCameraUp = glm::normalize(glm::cross(mCameraRight, mCameraFront)); // Create the new 'up' specific to the camera's orientation
}

void Camera::setupInputCallback()
{
  glfwSetWindowUserPointer(mWindow, this);

  glfwSetCursorPosCallback(mWindow, cursorCallback);
  glfwSetScrollCallback(mWindow, zoomCallback);

  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::mat4 Camera::getPerspective(float screenWidth, float screenHeight)
{
  // Prevent division by zero if window is minimized
  if (screenHeight == 0) screenHeight = 1;

  float aspectRatio = screenWidth / screenHeight;
  return glm::perspective(glm::radians(mFov), aspectRatio, 0.1f, 100.f);
}

void Camera::processInput(float deltaTime) {
  float cameraSpeed = static_cast<float>(2.5 * deltaTime);
  if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(mWindow, true);
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    cameraSpeed += 0.15f;
  if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
    mCameraPos += cameraSpeed * mCameraFront;
  if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
    mCameraPos -= cameraSpeed * mCameraFront;
  if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
    mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
  if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
    mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * cameraSpeed;
}
