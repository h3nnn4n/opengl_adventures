#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "light.h"
#include "manager.h"
#include "point_light_shadow.h"
#include "settings.h"
#include "shader_c.h"
#include "stb.h"

Shader *shader_pointlight_shadow = NULL;

void build_pointlight_shadow_maps() {
  shader_pointlight_shadow = newShader("shaders/pointlight_shadow/shader.vert",
                                       "shaders/pointlight_shadow/shader.frag",
                                       "shaders/pointlight_shadow/shader.geom");

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    Light *light = point_lights[i];

    if (light == NULL) continue;
    if (!light->active) continue;

    printf("%d\n", i);
    build_pointlight_shadow_map(light);
  }
}

void build_pointlight_shadow_map(Light *light) {
  assert(light);
  if (light == NULL) return;
  if (!light->active) return;

  printf("building light %p %d\n", light, light->point_light_index);

  glGenFramebuffers(1, &light->depthMapFBO);
  glGenTextures(1, &light->depthMap);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, light->depthMap);
  glObjectLabel(GL_TEXTURE, light->depthMap, -1, "cube shadow map");

  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 WINDOW_WIDTH * shadow_map_scale, WINDOW_WIDTH * shadow_map_scale,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, light->depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render_pointlight_shadow_maps() {
  assert(shader_pointlight_shadow);
  Shader_use(shader_pointlight_shadow);

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    render_pointlight_shadow_map(point_lights[i]);
  }
}

void render_pointlight_shadow_map(Light *light) {
  if (light == NULL) return;
  if (!light->active) return;

  assert(light->depthMapFBO >= 0);
  assert(light->depthMap >= 0);
  assert(shader_pointlight_shadow);

  glViewport(0, 0, WINDOW_WIDTH * shadow_map_scale, WINDOW_WIDTH * shadow_map_scale);
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  {
    glm_perspective(deg2rad(90.0f),
                    aspect_ratio,
                    near_plane,
                    far_plane,
                    light->shadow_projection);

    vec3 light_target[6] = {{ 1.0,  0.0,  0.0 },
                            {-1.0,  0.0,  0.0 },
                            { 0.0,  1.0,  0.0 },
                            { 0.0, -1.0,  0.0 },
                            { 0.0,  0.0,  1.0 },
                            { 0.0,  0.0, -1.0 }};

    vec3 light_up[6]     = {{ 0.0, -1.0,  0.0 },
                            { 0.0, -1.0,  0.0 },
                            { 0.0,  0.0,  1.0 },
                            { 0.0,  0.0, -1.0 },
                            { 0.0, -1.0,  0.0 },
                            { 0.0, -1.0,  0.0 }};

    for (int i = 0; i < 6; ++i) {
      vec3 target = GLM_VEC3_ZERO_INIT;

      glm_vec3_copy(light->position, target);
      glm_vec3_add(target, light_target[i], target);

      glm_lookat(light->position,
                target,
                light_up[i],
                light->shadow_views[i]);

      glm_mat4_mul(light->shadow_projection,
                  light->shadow_view,
                  light->light_space_matrixes[i]);

      char buffer[256];
      sprintf(buffer, "shadowMatrices[%d]", i);
      Shader_set_matrix4(shader_pointlight_shadow, buffer, (float*)light->light_space_matrixes[i]);
    }

    Shader_set_vec3(shader_pointlight_shadow, "lightPos", (float*)light->position);
    Shader_set_float(shader_pointlight_shadow, "far_plane", far_plane);
  }

  Manager_render_entities_with_shader(manager, shader_pointlight_shadow);
}

void bind_pointlight_shadow_map(Shader *shader, Light *light) {
  if (light == NULL) return;
  if (!light->active) return;

  assert(light->depthMapFBO >= 0);
  assert(light->depthMap >= 0);

  /*printf("debug light %p %d\n", light, light->depthMap);*/

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, light->depthMap);
  Shader_set_int(shader, "shadowCube", 2);

  Shader_set_float(shader, "far_plane", far_plane);
}
