#include "Camera.h"

void Camera::zoomCallback(GLFWwindow* mainWindow, double xoffset, double yoffset)
{
  Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(mainWindow));
  camera->fov -= static_cast<float>(yoffset);

  if (camera->fov < 1.f)
    camera->fov = 1.f;
  if (camera->fov > 60.f)
    camera->fov = 60.f;
}

void Camera::cursorCallback(GLFWwindow* mainWindow, double xposIn, double yposIn)
{
  Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(mainWindow));
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (camera->firstMouse) {
    camera->lastX = xpos;
    camera->lastY = ypos;
    camera->firstMouse = false;
  }

  float xOffSet = xpos - camera->lastX;
  float yOffSet = camera->lastY - ypos; //Reverse because y-coord go from bottom to top;

  camera->lastX = xpos;
  camera->lastY = ypos;

  const float sensitivity = 0.1f;
  xOffSet *= sensitivity;
  yOffSet *= sensitivity;

  camera->yaw += xOffSet;
  camera->pitch += yOffSet;

  if (camera->pitch > 89.f)
    camera->pitch = 89.f;
  if (camera->pitch < -89.f)
    camera->pitch = -89.f;

  glm::vec3 front{};

  front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
  front.y = sin(glm::radians(camera->pitch));
  front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

  camera->cameraFront = glm::normalize(front);
}

Camera::Camera(float screenWidth, float screenHeight, GLFWwindow* mainWindow) : 
  SCR_HEIGHT(screenHeight), SCR_WIDTH(screenWidth), window(mainWindow),
  pitch(0.f), yaw(-90.f), cameraFront(glm::vec3(0.f, -1.f, 0.f)),
  cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)), cameraUp(glm::vec3(0.f, 1.f, 0.f)),
  firstMouse(true), fov(45.f), lastX(SCR_WIDTH / 2.f), lastY(SCR_HEIGHT / 2.f)
{
}

void Camera::setupInputCallback()
{
  glfwSetWindowUserPointer(window, this);

  glfwSetCursorPosCallback(window, cursorCallback);
  glfwSetScrollCallback(window, zoomCallback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::mat4 Camera::getView()
{
  return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::getPerspective()
{
  return glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.f);;
}

void Camera::processInput(float deltaTime) {
  float cameraSpeed = static_cast<float>(2.5 * deltaTime);
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}
