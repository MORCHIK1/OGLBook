#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

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

  Shader cubeShader("cubeVertexShader.vert", "cubeFragmentShader.frag");
  Shader lightShader("lightVertexShader.vert", "lightFragmentShader.frag");

  Texture containerTexture("container2.png");
  Texture specularContainerTexture("container2_specular.png");

  camera.setupInputCallback();

  float vertices[] = {
    //Positions           //Normals           //Textures
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)
  };

  unsigned int VAOcube, VBO;

  glGenVertexArrays(1, &VAOcube);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAOcube);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  unsigned int VAOlight;

  glGenVertexArrays(1, &VAOlight);
  glBindVertexArray(VAOlight);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glm::vec3 ambient = glm::vec3(0.2f);
  glm::vec3 diffuse = glm::vec3(0.8f);
  glm::vec3 specular = glm::vec3(1.f);

  float linearTerm = 0.09f;
  float quadraticTerm = 0.032f;

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
    cubeShader.use();

    //Set Directional light (sun light)
    cubeShader.setVec3("dirLight.ambient", ambient);
    cubeShader.setVec3("dirLight.diffuse", diffuse);
    cubeShader.setVec3("dirLight.specular", specular);
    cubeShader.setVec3("dirLight.direction", camera.getCameraFront());

    // Set point light(street light)
    for (int i = 0; i < 4; ++i) { 
      cubeShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", ambient);
      cubeShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuse);
      cubeShader.setVec3("pointLights[" + std::to_string(i) + "].specular", specular);
      cubeShader.setVec3("pointLights[" + std::to_string(i) + "].position", camera.getCameraPos());
      //Set attenuation
      cubeShader.setFloat("pointLights[" + std::to_string(i) + "].constantTerm", 1.f);
      cubeShader.setFloat("pointLights[" + std::to_string(i) + "].linearTerm", linearTerm);
      cubeShader.setFloat("pointLights[" + std::to_string(i) + "].quadraticTerm", quadraticTerm);
    }

    //Set Spotlight (flashlight)
    cubeShader.setVec3("spotLight.ambient", ambient);
    cubeShader.setVec3("spotLight.diffuse", diffuse);
    cubeShader.setVec3("spotLight.specular", specular);
    cubeShader.setVec3("spotLight.direction", camera.getCameraFront());
    cubeShader.setVec3("spotLight.position", camera.getCameraPos());
    cubeShader.setFloat("spotLight.constantTerm", 1.f);
    cubeShader.setFloat("spotLight.linearTerm", linearTerm);
    cubeShader.setFloat("spotLight.quadraticTerm", quadraticTerm);
    cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(18.5f)));

    cubeShader.setVec3("viewPos", camera.getCameraPos());

    cubeShader.setInt("material.ambientDiffuse", 0);
    cubeShader.setInt("material.specular", 1);
    cubeShader.setFloat("material.shininess", 32.f);

    //Creating view, projection matrices for 3D and sending it to vertexShader
    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getPerspective(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
    glm::mat4 model = glm::mat4(1.f);

    cubeShader.setMat4("model", model);
    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("view", view);

    containerTexture.use(0);
    specularContainerTexture.use(1);

    glBindVertexArray(VAOcube);

    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle),
        glm::vec3(1.0f, 0.3f, 0.5f));
      cubeShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);

    glBindVertexArray(VAOlight);

    for (int i = 0; i < 4; ++i) {
      model = glm::mat4(1.f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

      lightShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAOcube);
  glDeleteVertexArrays(1, &VAOlight);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}