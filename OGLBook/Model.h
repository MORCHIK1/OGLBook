#pragma once
#include "Mesh.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Model
{
private:
  std::vector<Mesh> mMeshes;
  std::string mDirectory;
  std::vector<Texture> mTexturesLoaded;

  void loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
  Model(std::string path);
  void Draw(Shader& shader);
  unsigned int TextureFromFile(const char* path, const std::string& directory);
};

