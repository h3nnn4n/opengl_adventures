#include "shader.hpp"
#include "shader_c.h"

extern "C" {
  Shader* newShader(const char* vertexPath, const char* fragmentPath) {
    return new Shader(vertexPath, fragmentPath);
  }

  unsigned int Shader_get_id(Shader* shader) {
    return shader->ID;
  }

  void Shader_use(Shader* shader) {
    shader->use();
  }

  void Shader_set_bool(Shader* shader, const char* name, int value) {
    shader->setBool(name, value);
  }

  void Shader_set_int(Shader* shader, const char* name, int value) {
    shader->setInt(name, value);
  }

  void Shader_set_float(Shader* shader, const char* name, float value) {
    shader->setFloat(name, value);
  }
}
