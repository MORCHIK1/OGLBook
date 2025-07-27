#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>

#include "Shader.h"
//#include "Textures.h"
#include "Camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const float SCR_WIDTH = 800.0f;
const float SCR_HEIGHT = 600.0f;

//Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT), "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  Camera camera(window);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  camera.setupInputCallback();

  Model ourModel("model/backpack.obj");

  Shader ourShader("model.vert", "model.frag");

  glm::vec3 lightPosition = glm::vec3(4.f, 1.f, 2.f);

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.processInput(deltaTime);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    //Using shaders
    ourShader.use();

    ourShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    ourShader.setVec3("viewPos", camera.getCameraPos());

    //Set light
    ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("light.specular", 1.f, 1.f, 1.f);

    //Creating view, projection matrices for 3D and sending it to vertexShader
    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getPerspective(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    ourShader.setMat4("model", model);
    ourModel.Draw(ourShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}