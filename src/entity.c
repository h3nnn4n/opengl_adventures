#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stb.h>

#include "clickcolor.h"
#include "entity.h"
#include "model_c.h"
#include "model_catalog_c.h"
#include "shader_c.h"

#include "utils.h"

Entity* new_entity() {
  Entity* entity = malloc(sizeof(Entity));

  entity->data = NULL;
  entity->model  = NULL;
  entity->shader = NULL;
  entity->active = 1;
  entity->deleted = 0;
  entity->type = BLOCK;

  entity->model_path = NULL;
  entity->frag_shader_path = NULL;
  entity->vertex_shader_path = NULL;

  entity->shininess = 16;

  vec3 vec_one  = GLM_VEC3_ONE_INIT;
  vec3 vec_zero = GLM_VEC3_ZERO_INIT;

  glm_vec3_copy(vec_one , entity->color   );
  glm_vec3_copy(vec_one , entity->scale   );
  glm_vec3_copy(vec_zero, entity->rotation);
  glm_vec3_copy(vec_zero, entity->position);
  glm_vec3_copy(vec_zero, entity->color_id);

  generate_color_id(entity);

  return entity;
}

void destroy_entity(Entity *entity) {
  assert(entity);
  entity->deleted = 1;

  if (entity->data != NULL) free(entity->data);

  if (entity->model_path != NULL) free(entity->model_path);
  if (entity->frag_shader_path != NULL) free(entity->frag_shader_path);
  if (entity->vertex_shader_path != NULL) free(entity->vertex_shader_path);

  free(entity);
}

void draw_entity(Entity *entity) {
  assert(entity->model);
  assert(entity->shader);

  draw_entity_with_shader(entity, entity->shader);
}

void draw_entity_with_shader(Entity *entity, Shader *shader) {
  Shader_use(shader);

  mat4 identity_matrix = GLM_MAT4_IDENTITY_INIT;

  glm_mat4_copy(identity_matrix, entity->m_model);

  glm_translate(entity->m_model, entity->position);

  glm_rotate(entity->m_model, deg2rad(entity->rotation[0]), GLM_XUP);
  glm_rotate(entity->m_model, deg2rad(entity->rotation[1]), GLM_YUP);
  glm_rotate(entity->m_model, deg2rad(entity->rotation[2]), GLM_ZUP);

  glm_scale(entity->m_model, entity->scale);

  Shader_set_float(shader, "material.shininess", entity->shininess);
  Shader_set_vec3(shader, "material.color", (float*)entity->color);
  Shader_set_matrix4(shader, "model", (float*)entity->m_model);

  Model_draw(entity->model, shader);
}

void load_model(Entity *entity, char* model_path) {
  entity->model_path = malloc(sizeof(char) * (strlen(model_path) + 1));
  memcpy(entity->model_path, model_path, sizeof(char) * (strlen(model_path) + 1));

  entity->model = ModelCatalog_get_model(modelCatalog, model_path);
}
