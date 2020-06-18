#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include "box.h"
#include "camera.h"
#include "entity.h"
#include "light.h"
#include "manager.h"
#include "player.h"

Manager* manager;

Manager* init_manager() {
  Manager* manager = malloc(sizeof(Manager));

  manager->game_mode = IN_GAME;
  manager->game_state = PLAYING;

  manager->active_camera = NULL;
  manager->camera_count = 0;
  manager->max_cameras = 5;
  manager->cameras = malloc(sizeof(Camera*) * manager->max_cameras);

  manager->scene_sequence = NULL;
  manager->current_scene_index = 0;
  manager->current_scene_name = malloc(sizeof(char) * SCENE_NAME_BUFFER_SIZE);

  manager->entity_count = 0;
  manager->max_entities = 500;
  manager->entities = malloc(sizeof(Entity*) * manager->max_entities);

  memset(manager->cameras, 0, sizeof(Camera*) * manager->max_cameras);
  memset(manager->entities, 0, sizeof(Entity*) * manager->max_entities);
  memset(manager->current_scene_name, 0, sizeof(char) * SCENE_NAME_BUFFER_SIZE);

  return manager;
}

void Manager_add_camera(Manager *manager, Camera *camera) {
  assert(manager);
  assert(camera);

  assert(manager->camera_count < manager->max_cameras);

  manager->cameras[manager->camera_count] = camera;
  manager->camera_count++;
}

void Manager_set_active_camera(Manager *manager, int camera_index) {
  assert(manager);

  assert(camera_index < manager->camera_count);

  manager->active_camera = manager->cameras[camera_index];
}

void Manager_tick_timer(Manager* manager) {
  manager->current_time = glfwGetTime();

  manager->last_frame_time = manager->current_frame_time;
  manager->current_frame_time = manager->current_time;
  manager->delta_time = manager->current_frame_time - manager->last_frame_time;
}

void Manager_add_entity(Manager *manager, Entity *entity) {
  assert(manager);
  assert(entity);

  for (int i = 0; i < manager->entity_count; ++i) {
    if (manager->entities[i] != NULL) continue;

    manager->entities[i] = entity;
    /*printf("foudn a gap at %p %i\n", manager->entities[i], i);*/
    return;
  }

  assert(manager->entity_count < manager->max_entities);

  manager->entities[manager->entity_count] = entity;
  manager->entity_count++;
}

void Manager_destroy_entity(Manager *manager, Entity *entity) {
  for (int i = 0; i < manager->entity_count; ++i) {
    if (entity == manager->entities[i]) {
      destroy_entity(entity);
      manager->entities[i] = NULL;
    }
  }
}

void Manager_destroy_camera(Manager *manager, Camera *camera) {
  for (int i = 0; i < manager->camera_count; ++i) {
    if (camera == manager->cameras[i]) {
      destroy_camera(camera);
      manager->cameras[i] = NULL;
    }
  }
}

void Manager_render_entities(Manager *manager) {
  assert(manager);

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    if (entity == NULL) continue;
    if (!entity->active) continue;
    if (entity->deleted) continue;

    update_camera_projection_matrix(manager->active_camera, entity->shader);
    update_camera_view_matrix(manager->active_camera, entity->shader);

    draw_entity(entity);
  }
}

void Manager_update_active_camera_location(Manager *manager) {
  assert(manager);
  assert(manager->active_camera);

  update_camera_position_matrix(manager->active_camera);
}

Entity* Manager_get_first_entity_by_type(Manager *manager, EntityType type) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    if (entity->type == type) return entity;
  }

  return NULL;
}

Entity* Manager_get_entity_by_type(Manager *manager, EntityType type, Entity *starting_entity) {
  if (starting_entity == NULL) return Manager_get_first_entity_by_type(manager, type);

  int found_last = false;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;
    if (entity == starting_entity) {
      found_last = true;
      continue;
    }

    if (entity->type == type && found_last) return entity;
  }

  return NULL;
}

void Manager_update_entities(Manager *manager) {
  if (manager->game_mode == EDITOR) return;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    if (entity->type == PLAYER) player_update(entity);
    if (entity->type == BOX) box_update(entity);
  }
}

int Manager_has_entity_at_position(Manager *manager, vec3 position) {
  return Manager_entity_at_position(manager, position) != NULL;
}

int Manager_entity_at_position_count(Manager *manager, vec3 position) {
  int count = 0;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    if (glm_vec3_eqv_eps(entity->position, position)) {
      count += 1;
    }
  }

  return count;
}

Entity* Manager_entity_at_position(Manager *manager, vec3 position) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    if (glm_vec3_eqv_eps(entity->position, position)) {
      return entity;
    }
  }

  return NULL;
}

Entity* Manager_entity_at_position_of_type(Manager *manager, vec3 position, EntityType type) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    if (entity->type == type && glm_vec3_eqv_eps(entity->position, position)) {
      return entity;
    }
  }

  return NULL;
}

Entity* Manager_next_entity_at_position(Manager *manager, vec3 position, Entity *last_entity) {
  int found_first = 0;

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;
    if (entity == last_entity) {
      found_first = 1;
      continue;
    }

    if (found_first && glm_vec3_eqv_eps(entity->position, position)) {
      return entity;
    }
  }

  return NULL;
}

void Manager_destroy_entities(Manager *manager) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];
    if (entity == NULL) continue;

    destroy_entity(entity);

    entity->deleted = 1;
    manager->entities[entity_index] = NULL;
  }
}

void Manager_destroy_cameras(Manager *manager) {
  for (int i = 0; i < manager->camera_count; ++i) {
    Camera *camera = manager->cameras[i];
    if (camera == NULL) continue;

    destroy_camera(camera);
  }

  manager->camera_count = 0;
}

void Manager_destroy_lights(Manager *manager) {
  if (directional_light != NULL) destroy_light(directional_light);
  if (spot_light != NULL) destroy_light(spot_light);

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    Light *light = point_lights[i];

    if (light != NULL) free(light);
  }
}
