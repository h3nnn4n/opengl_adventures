#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "clickcolor_rendering.h"
#include "clickcolor.h"
#include "entity.h"
#include "input_handling.h"
#include "manager.h"
#include "stb.h"

void clickcolor_event() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  vec3 pixel;
  glReadPixels(lastX, lastY, 1, 1, GL_RGB, GL_FLOAT, &pixel);

  float distance = 100;
  Entity *best_match = NULL;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    float new_distance = glm_vec3_distance(pixel, entity->color_id);

    if (new_distance < distance) {
      distance = new_distance;
      best_match = entity;
    }
  }

  printf("%6.3f %6.3f %6.3f\n", pixel[0], pixel[1], pixel[2]);
  printf("%6.3f %6.3f %6.3f\n", best_match->color_id[0], best_match->color_id[1], best_match->color_id[2]);

  best_match->active = 0;
}

void generate_color_id(Entity *entity) {
  float distance_threshold = 0.025;
  float distance = 100;

  do {
    entity->color_id[0] = stb_frand();
    entity->color_id[1] = stb_frand();
    entity->color_id[2] = stb_frand();

    distance = 100;

    for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
      Entity *entity_ = manager->entities[entity_index];

      float new_distance = glm_vec3_distance(entity->color_id, entity_->color_id);

      if (new_distance < distance) {
        distance = new_distance;
      }
    }
  } while (distance < distance_threshold && manager->entity_count > 0);

  printf("%f %f\n", distance, distance_threshold);
}
