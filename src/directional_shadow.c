#include <stddef.h>

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "directional_shadow.h"
#include "light.h"
#include "manager.h"
#include "settings.h"
#include "shader_c.h"

Shader *shader_directional_shadow;

void build_directional_shadow_map(Light *light) {
  if (light == NULL) return;
  if (!light->active) return;

  glGenFramebuffers(1, &light->depthMapFBO);
  glGenTextures(1, &light->depthMap);
  glBindTexture(GL_TEXTURE_2D, light->depthMap);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               WINDOW_WIDTH * shadow_map_scale, WINDOW_HEIGHT * shadow_map_scale,
               0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  shader_directional_shadow = newShader("shaders/directional_shadow/shader.vert",
                                        "shaders/directional_shadow/shader.frag",
                                        NULL);
}

void render_directional_shadow_map(Light *light) {
  if (light == NULL) return;
  if (!light->active) return;

  Shader_use(shader_directional_shadow);

  glViewport(0, 0, WINDOW_WIDTH * shadow_map_scale, WINDOW_HEIGHT * shadow_map_scale);
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  {
    float light_distance = 10.0f;

    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, light->shadow_projection);

    vec3 shadow_target = GLM_VEC3_ZERO_INIT;
    vec3 shadow_up = GLM_VEC3_ZERO_INIT;
    shadow_up[1] = 1.0f;

    vec3 shadow_pos = GLM_VEC3_ZERO_INIT;

    {
      vec3 flipped_direction = GLM_VEC3_ZERO_INIT;
      glm_vec3_negate_to(light->direction, flipped_direction);

      glm_vec3_scale_as(flipped_direction, light_distance, shadow_pos);
    }

    glm_lookat(shadow_pos,
               shadow_target,
               shadow_up,
               light->shadow_view);

    glm_mat4_mul(light->shadow_projection,
                 light->shadow_view,
                 light->light_space_matrix);

    Shader_set_matrix4(shader_directional_shadow, "projection", (float*)light->shadow_projection);
    Shader_set_matrix4(shader_directional_shadow, "view", (float*)light->shadow_view);
    Shader_set_matrix4(shader_directional_shadow, "lightSpaceMatrix", (float*)light->light_space_matrix);
  }

  Manager_render_entities_with_shader(manager, shader_directional_shadow);
}

void bind_directional_shadow_map(Shader *shader, Light *light) {
  if (light == NULL) return;
  if (!light->active) return;

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, light->depthMap);
  Shader_set_int(shader, "shadowMap", 1);
}
