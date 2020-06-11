#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include <stb.h>

#include "clickcolor_rendering.h"
#include "clickcolor.h"
#include "editor.h"
#include "entity.h"
#include "input_handling.h"
#include "manager.h"
#include "settings.h"

void clickcolor_event() {
  vec3 pixel;
  vec3 black = GLM_VEC3_ZERO_INIT;
  vec3 normal = GLM_VEC3_ZERO_INIT;

  glBindFramebuffer(GL_FRAMEBUFFER, color_id_framebuffer);
  glReadPixels(lastX, WINDOW_HEIGHT - lastY, 1, 1, GL_RGB, GL_FLOAT, &pixel);

  glBindFramebuffer(GL_FRAMEBUFFER, normals_framebuffer);
  glReadPixels(lastX, WINDOW_HEIGHT - lastY, 1, 1, GL_RGB, GL_FLOAT, &normal);

  if (glm_vec3_distance(pixel, black) < 0.001) {
    return;
  }

  float distance = 1;
  Entity *best_match = NULL;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    if (entity == NULL) continue;
    if (!entity->active) continue;
    if (entity->deleted) continue;

    float new_distance = glm_vec3_distance(pixel, entity->color_id);

    if (new_distance < distance) {
      distance = new_distance;
      best_match = entity;
    }
  }

  selected_entity.entity = best_match;

  glm_vec3_copy(normal, selected_entity.normal);

  selected_entity.screen_click_position[0] = lastX;
  selected_entity.screen_click_position[1] = lastY;
}

void generate_color_id(Entity *entity) {
  float distance_threshold = 0.005;
  float distance;

  do {
    entity->color_id[0] = stb_frand();
    entity->color_id[1] = stb_frand();
    entity->color_id[2] = stb_frand();

    distance = 1;

    for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
      Entity *entity_ = manager->entities[entity_index];
      if (entity_ == NULL) continue;

      float new_distance = glm_vec3_distance(entity->color_id, entity_->color_id);

      if (new_distance < distance) {
        distance = new_distance;
      }
    }
  } while (distance < distance_threshold && manager->entity_count > 0);
}
