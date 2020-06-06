#include <stdlib.h>
#include <assert.h>

#include "entity.h"

#include "shader_c.h"
#include "model_c.h"

#include "utils.h"

Entity* new_entity() {
  Entity* entity = malloc(sizeof(Entity));

  entity->model  = NULL;
  entity->shader = NULL;

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

  Shader_use(entity->shader);

  mat4 identity_matrix = GLM_MAT4_IDENTITY_INIT;

  glm_mat4_copy(identity_matrix, entity->m_model);

  glm_translate(entity->m_model, entity->position);

  glm_rotate(entity->m_model, deg2rad(entity->rotation[0]), GLM_XUP);
  glm_rotate(entity->m_model, deg2rad(entity->rotation[1]), GLM_YUP);
  glm_rotate(entity->m_model, deg2rad(entity->rotation[2]), GLM_ZUP);

  glm_scale(entity->m_model, entity->scale);

  Shader_set_matrix4(entity->shader, "model", (float*)entity->m_model);

  Model_draw(entity->model, entity->shader);
}

void load_model(Entity *entity, char* model_path) {
  entity->model = newModel(model_path);
}
