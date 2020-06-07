#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "entity.h"

#include "shader_c.h"
#include "model_c.h"

#include "utils.h"

Entity* new_entity() {
  Entity* entity = malloc(sizeof(Entity));

  entity->model  = NULL;
  entity->shader = NULL;
  entity->active = 1;

  vec3 vec_one  = GLM_VEC3_ONE_INIT;
  vec3 vec_zero = GLM_VEC3_ZERO_INIT;

  glm_vec3_copy(vec_one , entity->scale   );
  glm_vec3_copy(vec_zero, entity->rotation);
  glm_vec3_copy(vec_zero, entity->position);

  return entity;
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

  Shader_set_matrix4(shader, "model", (float*)entity->m_model);

  Model_draw(entity->model, shader);
}

void load_model(Entity *entity, char* model_path) {
  entity->model_path = malloc(sizeof(char) * (strlen(model_path) + 1));
  memcpy(entity->model_path, model_path, sizeof(char) * (strlen(model_path) + 1));

  entity->model = newModel(model_path);
}
