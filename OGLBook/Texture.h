#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "stb_image.h"

class Texture {
private:

  unsigned int textureID;

  static const std::string BASE_TEXTURE_DIR;

  std::string m_imageFileName;

public:
  Texture(const std::string imageFile);

  void use() const;

  const std::string& getImageFileName() const { return m_imageFileName; }

  ~Texture();
};


#endif