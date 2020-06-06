#include <stdlib.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "entity.h"
#include "manager.h"

Manager* manager;

Manager* init_manager() {
  Manager* manager = malloc(sizeof(Manager));

  manager->active_camera = NULL;
  manager->camera_count = 0;
  manager->max_cameras = 5;
  manager->cameras = malloc(sizeof(Camera*) * manager->max_cameras);

  manager->entity_count = 0;
  manager->max_entities = 50;
  manager->entities = malloc(sizeof(Entity*) * manager->max_entities);

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

  assert(manager->entity_count < manager->max_entities);

  manager->entities[manager->entity_count] = entity;
  manager->entity_count++;
}

void Manager_render_entities(Manager *manager) {
  assert(manager);

  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

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