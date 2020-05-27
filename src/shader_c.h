#ifndef SHADER_C_H
#define SHADER_C_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct Shader Shader;

  Shader* newShader(const char* vertexPath, const char* fragmentPath);

  void Shader_use(Shader* shader);
  void Shader_set_bool(Shader* shader, const char* name, int value);
  void Shader_set_int(Shader* shader, const char* name, int value);
  void Shader_set_float(Shader* shader, const char* name, float value);

  void deleteMyClass(Shader* shader);

#ifdef __cplusplus
}
#endif

#endif
