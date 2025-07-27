#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int ID;
  std::string Type;
  std::string Path;
};

class Mesh
{
private:
  std::vector<Vertex>        mVertices;
  std::vector<unsigned int>  mIndices;
  std::vector<Texture>       mTextures;

  unsigned int mVAO;
  unsigned int mVBO;
  unsigned int mEBO;

  void setupMesh();

public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

  void Draw(Shader& shader);

  const std::vector<Vertex>& getVertices() const { return mVertices; }

  const std::vector<unsigned int>& getIndices() const { return mIndices; }

  const std::vector<Texture>& getTextures() const { return mTextures; }
};

