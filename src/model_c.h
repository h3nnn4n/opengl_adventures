#ifndef _MODEL_C_H
#define _MODEL_C_H

#include "shader_c.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct Model Model;

  Model* newModel(char* path);
  void Model_draw(Model* model, Shader* shader);

#ifdef __cplusplus
}
#endif

#endif
