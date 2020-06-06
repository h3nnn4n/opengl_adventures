#ifndef _MANAGER_H
#define _MANAGER_H

#include "camera.h"
#include "entity.h"

typedef struct {
  /////////////////
  // Camera Stuff
  //
  Camera* active_camera;
  Camera** cameras;

  int camera_count;
  int max_cameras;

  /////////////////
  // Timer Stuff
  //
  float delta_time;
  float current_time;
  float current_frame_time;
  float last_frame_time;

  /////////////////
  // Entities
  //
  Entity** entities;

  int entity_count;
  int max_entities;
} Manager;

extern Manager* manager;

Manager* init_manager();

void Manager_set_active_camera(Manager *manager, int camera_index);
void Manager_add_camera(Manager *manager, Camera *camera);

void Manager_tick_timer(Manager* manager);

void Manager_add_entity(Manager *manager, Entity *entity);
void Manager_render_entities(Manager *manager);

void Manager_update_active_camera_location(Manager *manager);

#endif
