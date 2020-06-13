#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "model_catalog.hpp"

Model* ModelCatalog::get_model(char* &path) {
  for (unsigned int i = 0; i < model_catalog.size(); ++i) {
    if (std::strcmp(model_catalog[i]->path.data(), path) == 0) {
      return model_catalog[i];
    }
  }

  Model *model = new Model(path);

  model_catalog.push_back(model);

  return model;
}

unsigned int ModelCatalog::get_texture_id(const char* path, string typeName) {
  for(unsigned int i = 0; i < texture_catalog.size(); i++) {
    if (std::strcmp(texture_catalog[i].data(), path) == 0) {
      return texture_catalog_id[i];
    }
  }

  unsigned int texture_id = TextureFromFile(path, typeName);

  texture_catalog.push_back(string(path));
  texture_catalog_id.push_back(texture_id);

  return texture_id;
}

unsigned int TextureFromFile(const char *path, string typeName) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

  if (data) {
    GLenum format;
    GLenum internal_format;
    int use_gamma = typeName.compare("texture_height") == 0;

    if (nrComponents == 1) {
      format = GL_RED;
    } else if (nrComponents == 3) {
      format = GL_RGB;
    } else if (nrComponents == 4) {
      format = GL_RGBA;
    } else {
      format = GL_RGB;
    }

    internal_format = format;

    if (use_gamma) {
      if (nrComponents == 3) {
        internal_format = GL_SRGB;
      } else if (nrComponents == 4) {
        internal_format = GL_SRGB_ALPHA;
      }
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
