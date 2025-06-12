#include "Texture.h"

const std::string Texture::BASE_TEXTURE_DIR = "textures/";

Texture::Texture(const std::string imageFile) : m_imageFileName(imageFile)
{
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  std::string pathToTexture = BASE_TEXTURE_DIR + m_imageFileName;

  stbi_set_flip_vertically_on_load(true);

  unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height,
    &nrChannels, 0);
    
  if (data) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    else {

      std::cerr << "WARNING::TEXTURE::UNSUPPORTED_IMAGE_FORMAT: " << pathToTexture << " (channels: " << nrChannels << ")" << std::endl;
      stbi_image_free(data);
      return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
      0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "FAILED TO LOAD TEXTURE" << "\n";
  }

  stbi_image_free(data);
}

void Texture::use(unsigned int textureUnit) const
{
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture() {
  glDeleteTextures(1, &textureID);
}

