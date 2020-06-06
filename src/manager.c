#include <stdlib.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include "manager.h"
#include "camera.h"

Manager* manager;

Manager* init_manager() {
  Manager* manager = malloc(sizeof(Manager));
  manager->camera_count = 0;
  manager->max_cameras = 5;
  manager->cameras = malloc(sizeof(Camera*) * manager->max_cameras);

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
