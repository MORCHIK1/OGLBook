#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "stb_image.h"

class Texture {
public:
  unsigned int textureID;

  Texture(const char* imageFile);

  void use() const;

  ~Texture();
};


#endif