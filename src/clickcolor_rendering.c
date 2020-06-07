#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "clickcolor_rendering.h"
#include "manager.h"
#include "settings.h"

unsigned int framebuffer;
unsigned int texColorBuffer;

Shader *shader_colorclicker;

void clickcolor_render_pass() {
  Shader_reload_changes(shader_colorclicker);

  // Bind to the funky click color texture
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Clear color and depth buffers
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draws
  Shader_use(shader_colorclicker);

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    if (entity == NULL) continue;
    if (!entity->active) continue;

    update_camera_projection_matrix(manager->active_camera, shader_colorclicker);
    update_camera_view_matrix(manager->active_camera, shader_colorclicker);

    draw_entity_with_shader(entity, shader_colorclicker);
  }
}

void build_clickcolor_framebuffer() {
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // generate texture
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  shader_colorclicker = newShader("shaders/clickcolor/shader.vert",
                                  "shaders/clickcolor/shader.frag");
}
