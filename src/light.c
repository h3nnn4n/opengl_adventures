#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include <stb.h>

#include "light.h"
#include "shader_c.h"
#include "utils.h"

Light *directional_light = NULL;
Light *spot_light = NULL;
Light *point_lights[NR_POINT_LIGHTS] = {NULL, NULL, NULL, NULL};

Light* make_light(LightType type) {
  Light *light = malloc(sizeof(Light));

  light->light_type = type;

  mat4 identity = GLM_MAT4_IDENTITY_INIT;
  glm_mat4_copy(identity, light->shadow_projection);
  glm_mat4_copy(identity, light->shadow_view);
  glm_mat4_copy(identity, light->light_space_matrix);

  for (int i = 0; i < 6; ++i) {
    glm_mat4_copy(identity, light->shadow_views[i]);
    glm_mat4_copy(identity, light->light_space_matrixes[i]);
  }

  vec3 tmp = GLM_VEC3_ZERO_INIT;

  glm_vec3_copy(tmp, light->position);
  glm_vec3_copy(tmp, light->direction);

  glm_vec3_broadcast(0.02f, tmp);
  glm_vec3_copy(tmp, light->ambient);

  glm_vec3_broadcast(0.3f, tmp);
  glm_vec3_copy(tmp, light->specular);

  glm_vec3_broadcast(0.4f, tmp);
  glm_vec3_copy(tmp, light->diffuse);

  light->constant = 1.0f;
  light->linear = 0.09f;
  light->quadratic = 0.032f;

  light->cutOff = 12.5f;
  light->outterCutOff = 17.5f;

  light->shader = NULL;
  light->active = 0;

  light->depthMapFBO = -1;
  light->depthMap = -1;

  return light;
}

void destroy_light(Light *light) {
  assert(light);
  free(light);
  light = NULL;
}

void refresh_lights() {
  if (directional_light != NULL) refresh_light(directional_light);
  if (spot_light != NULL) refresh_light(spot_light);

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    Light *light = point_lights[i];

    if (light != NULL) {
      light->point_light_index = i;
      refresh_light(light);
    }
  }
}

void set_position(Light *light, float* value) {
  memcpy(light->position, value, sizeof(vec3));
}

void set_direction(Light *light, float* value) {
  memcpy(light->direction, value, sizeof(vec3));
}

void set_ambient(Light *light, float* value) {
  memcpy(light->ambient, value, sizeof(vec3));
}

void set_specular(Light *light, float* value) {
  memcpy(light->specular, value, sizeof(vec3));
}

void set_diffuse(Light *light, float* value) {
  memcpy(light->diffuse, value, sizeof(vec3));
}

void refresh_light(Light *light) {
  Shader *shader = light->shader;
  assert(shader);


  switch (light->light_type) {
    case SPOTLIGHT:
      /*fprintf(stdout, "refreshing spotlight\n");*/
      Shader_set_int(shader, "spotLight.active", light->active);

      if (!light->active) return;

      Shader_set_vec3(shader, "spotLight.position", (float*)light->position);
      Shader_set_vec3(shader, "spotLight.direction", (float*)light->direction);
      Shader_set_vec3(shader, "spotLight.ambient", (float*)light->ambient);
      Shader_set_vec3(shader, "spotLight.diffuse", (float*)light->diffuse);
      Shader_set_vec3(shader, "spotLight.specular", (float*)light->specular);
      Shader_set_float(shader, "spotLight.constant", light->constant);
      Shader_set_float(shader, "spotLight.linear", light->linear);
      Shader_set_float(shader, "spotLight.quadratic", light->quadratic);
      Shader_set_float(shader, "spotLight.cutOff", cos(deg2rad(light->cutOff)));
      Shader_set_float(shader, "spotLight.outterCutOff", cos(deg2rad(light->outterCutOff)));
      break;
    case DIRECTIONAL:
      /*fprintf(stdout, "refreshing direcional light\n");*/
      Shader_set_int(shader, "dirLight.active", light->active);

      if (!light->active) return;

      Shader_set_matrix4(shader, "lightSpaceMatrix", (float*)light->light_space_matrix);

      Shader_set_vec3(shader, "dirLight.direction", (float*)light->direction);
      Shader_set_vec3(shader, "dirLight.ambient", (float*)light->ambient);
      Shader_set_vec3(shader, "dirLight.diffuse", (float*)light->diffuse);
      Shader_set_vec3(shader, "dirLight.specular", (float*)light->specular);
      break;
    case POINT:
      {
        int light_index = light->point_light_index;
        char buffer[256];

        /*fprintf(stdout, "refreshing point light %d\n", light_index);*/

        sprintf(buffer, "pointLights[%d].active", light_index);
        Shader_set_int(shader, buffer, light->active);

        if (!light->active) return;

        if (light_index == 2)
          for (int i = 0; i < 6; ++i) {
            sprintf(buffer, "shadowMatrices[%d]", i);
            Shader_set_matrix4(shader, buffer, (float*)light->light_space_matrixes[i]);
          }

        sprintf(buffer, "pointLights[%d].position", light_index);
        Shader_set_vec3(shader, buffer, (float*)light->position);

        sprintf(buffer, "pointLights[%d].ambient", light_index);
        Shader_set_vec3(shader, buffer, (float*)light->ambient);

        sprintf(buffer, "pointLights[%d].diffuse", light_index);
        Shader_set_vec3(shader, buffer, (float*)light->diffuse);

        sprintf(buffer, "pointLights[%d].specular", light_index);
        Shader_set_vec3(shader, buffer, (float*)light->specular);

        sprintf(buffer, "pointLights[%d].constant", light_index);
        Shader_set_float(shader, buffer, light->constant);

        sprintf(buffer, "pointLights[%d].linear", light_index);
        Shader_set_float(shader, buffer, light->linear);

        sprintf(buffer, "pointLights[%d].quadratic", light_index);
        Shader_set_float(shader, buffer, light->quadratic);
      }
      break;
  }
}

void draw_point_lights() {
  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    Light *light = point_lights[i];

    if (light == NULL || !light->active || !light->draw) continue;

    Shader_use(light->shader_light_obj);

    vec3 v_scale = { 0.2, 0.2, 0.2 };
    mat4 m_model = GLM_MAT4_IDENTITY_INIT;

    glm_translate(m_model, light->position);
    glm_scale(m_model, v_scale);

    Shader_set_matrix4(light->shader_light_obj, "model", (float*)m_model);

    Shader_set_vec3(light->shader_light_obj, "lightColor", (float*)light->diffuse);

    Model_draw(light->model, light->shader_light_obj);
  }
}

void load_light_model(Light *light, Shader *shader, char* model_path) {
  light->model = newModel(model_path);
  light->shader_light_obj = shader;
}
