#ifndef _ENTITY_H
#define _ENTITY_H

#include <cglm/cglm.h>

#include "shader_c.h"
#include "model_c.h"

typedef struct {
  Model *model;
  Shader *shader;

  char *model_path;
  char *frag_shader_path;
  char *vertex_shader_path;

  vec3 position;
  vec3 scale;
  vec3 rotation;

  mat4 m_model;

  int active;

  vec3 color_id;
} Entity;

Entity* new_entity();
void draw_entity(Entity *entity);
void draw_entity_with_shader(Entity *entity, Shader *shader);
void load_model(Entity *entity, char* model_path);

#endif
