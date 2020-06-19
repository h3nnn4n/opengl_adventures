#include <stddef.h>

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "directional_shadow.h"
#include "light.h"
#include "manager.h"
#include "settings.h"
#include "shader_c.h"

unsigned int depthMapFBO;
unsigned int depthMap;

unsigned int shadow_map_scale = 4;

Shader *shader_directional_shadow;

void build_directional_shadow_map() {
  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthMap);

  glBindTexture(GL_TEXTURE_2D, depthMap);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               WINDOW_WIDTH * shadow_map_scale, WINDOW_HEIGHT * shadow_map_scale,
               0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  shader_directional_shadow = newShader("shaders/directional_shadow/shader.vert",
                                        "shaders/directional_shadow/shader.frag");
}

void render_directional_shadow_map() {
  if (directional_light == NULL) return;

  Shader_use(shader_directional_shadow);

  glViewport(0, 0, WINDOW_WIDTH * shadow_map_scale, WINDOW_HEIGHT * shadow_map_scale);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  {
    float light_distance = 10.0f;
    float near_plane = 1.0f;
    float far_plane = 25.0f;

    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, directional_light->shadow_projection);

    vec3 shadow_target = GLM_VEC3_ZERO_INIT;
    vec3 shadow_up = GLM_VEC3_ZERO_INIT;
    shadow_up[1] = 1.0f;

    vec3 shadow_pos = GLM_VEC3_ZERO_INIT;

    {
      vec3 flipped_direction = GLM_VEC3_ZERO_INIT;
      glm_vec3_negate_to(directional_light->direction, flipped_direction);

      glm_vec3_scale_as(flipped_direction, light_distance, shadow_pos);
    }

    glm_lookat(shadow_pos,
               shadow_target,
               shadow_up,
               directional_light->shadow_view);

    glm_mat4_mul(directional_light->shadow_projection,
                 directional_light->shadow_view,
                 directional_light->light_space_matrix);

    Shader_set_matrix4(shader_directional_shadow, "projection", (float*)directional_light->shadow_projection);
    Shader_set_matrix4(shader_directional_shadow, "view", (float*)directional_light->shadow_view);
    Shader_set_matrix4(shader_directional_shadow, "lightSpaceMatrix", (float*)directional_light->light_space_matrix);
  }

  Manager_render_entities_with_shader(manager, shader_directional_shadow);
}

void bind_directional_shadow_map(Shader *shader) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  Shader_set_int(shader, "shadowMap", 1);
}
