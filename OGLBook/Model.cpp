#include "Model.h"

void Model::loadModel(std::string path)
{
  Assimp::Importer import;

  const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);

  //Explanation for if statement:
  //For !scene: "Did we get anything back from the loader at all?"
  // !!!!!!!!!!!!!!!!
  //For scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE: "Even though we loaded something, is the data known to be broken or incomplete?" 
  //AI_SCENE_FLAGS_INCOMPLETE: This is a constant defined by Assimp that represents a single bit
  //scene->mFlags is an integer that acts as a bitfield
  // !!!!!!!!!!!!!!!!
  //!scene->mRootNode: "Does the loaded scene actually contain a scene hierarchy, or is it empty?"
  //scene->mRootNode: The aiScene object contains a pointer to the root node (aiNode) of the scene's hierarchy. 
  //This node is the entry point to all the meshes, lights, cameras, and other objects in the scene.
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP:: " << import.GetErrorString() << std::endl;
    return;
  }

  mDirectory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  //Process all the node's meshes
  for (int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    mMeshes.push_back(processMesh(mesh, scene));
  }
  //Processing each of its children
  for (int i = 0; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  
  for (int i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;

    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;

    vertex.Position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;

    vertex.Normal = vector;

    if (mesh->mTextureCoords[0]) {
      glm::vec2 texVec;

      texVec.x = mesh->mTextureCoords[0][i].x;
      texVec.y = mesh->mTextureCoords[0][i].y;

      vertex.TexCoords = texVec;
    }
    else {
      vertex.TexCoords = glm::vec2(0.f);
    }

    vertices.push_back(vertex);
  }


  for (int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> textures;

  for (int i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool skip = false;
    //Checking if the texture that we are trying to load already exists in mTexturesLoaded vector
    for (int j = 0; j < mTexturesLoaded.size(); ++j) {
      if (std::strcmp(mTexturesLoaded[j].Path.data(), str.C_Str()) == 0) {
        textures.push_back(mTexturesLoaded[j]);
        skip = true;
        break;
      }
    }
    //If texture wasn't loaded load it and add it to the vector
    if (!skip) {
      Texture texture;
      texture.ID = TextureFromFile(str.C_Str(), this->mDirectory);
      texture.Type = typeName;
      texture.Path = str.C_Str();
      textures.push_back(texture);
      mTexturesLoaded.push_back(texture);
    }
  }
  return textures;
}

Model::Model(std::string path)
{
  loadModel(path);
}

void Model::Draw(Shader& shader)
{
  for (int i = 0; i < mMeshes.size(); ++i) {
    mMeshes[i].Draw(shader);
  }
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}