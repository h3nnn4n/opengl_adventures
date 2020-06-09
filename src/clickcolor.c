#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "clickcolor_rendering.h"
#include "clickcolor.h"
#include "editor.h"
#include "entity.h"
#include "input_handling.h"
#include "manager.h"
#include "settings.h"
#include "stb.h"

void clickcolor_event() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  vec3 black = GLM_VEC3_ZERO_INIT;

  vec3 pixel;
  glReadPixels(lastX, WINDOW_HEIGHT - lastY, 1, 1, GL_RGB, GL_FLOAT, &pixel);

  if (glm_vec3_distance(pixel, black) < 0.001) {
    return;
  }

  float distance = 1;
  Entity *best_match = NULL;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    float new_distance = glm_vec3_distance(pixel, entity->color_id);

    if (new_distance < distance) {
      distance = new_distance;
      best_match = entity;
    }
  }

  selected_entity=best_match;
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

      float new_distance = glm_vec3_distance(entity->color_id, entity_->color_id);

      if (new_distance < distance) {
        distance = new_distance;
      }
    }
  } while (distance < distance_threshold && manager->entity_count > 0);
}
