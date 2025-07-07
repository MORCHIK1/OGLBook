#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "stb_image.h"

class Textures {
private:
  unsigned int mTextureID;

  static const std::string BASE_TEXTURE_DIR;

  std::string m_imageFileName;

public:
  Textures(const std::string imageFile);

  void use(unsigned int textureUnit) const;

  const std::string& getImageFileName() const { return m_imageFileName; }

  unsigned int getID() { return mTextureID; }

  ~Textures();

};