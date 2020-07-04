#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "clickcolor_rendering.h"
#include "directional_shadow.h"
#include "entity.h"
#include "gui.h"
#include "input_handling.h"
#include "light.h"
#include "manager.h"
#include "point_light_shadow.h"
#include "settings.h"
#include "shader_c.h"

void main_render_pass() {
  // Bind to screen buffer
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Clear screen
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (wireframe_mode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  Shader_use(manager->default_shader);

  bind_directional_shadow_map(manager->default_shader, directional_light);
  bind_pointlight_shadow_map(manager->default_shader, point_lights[2]);

  // Lights
  refresh_lights();

  // Draws
  Manager_render_entities(manager);

  // Lights (render light positions)
  if (manager->game_mode == EDITOR) {
    Shader_use(manager->default_shader_light);
    update_camera_projection_matrix(manager->active_camera, manager->default_shader_light);
    update_camera_view_matrix(manager->active_camera, manager->default_shader_light);
    draw_point_lights();
  }
}
