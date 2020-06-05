#ifndef _ENTITY_H
#define _ENTITY_H

#include <cglm/cglm.h>

#include "shader_c.h"
#include "model_c.h"

typedef struct {
  Model *model;
  Shader *shader;

  vec3 position;
  vec3 scale;
  vec3 rotation;

  mat4 m_model;
} Entity;

Entity* new_entity();
void draw_entity(Entity *entity);
void load_model(Entity *entity, char* model_path);

#endif
