#ifndef _MANAGER_H
#define _MANAGER_H

#include "camera.h"
#include "entity.h"

#define SCENE_NAME_BUFFER_SIZE 256

typedef enum {
  IN_GAME,
  IN_MENU,
  FREE_CAMERA,
  EDITOR
} GameMode;

typedef enum {
  PLAYING,
  WON,
} GameState;

typedef struct {
  /////////////////
  // Stuff Stuff, aka HACK, aka being lazy
  // aka getting things done asap
  // then fixing it later
  //
  Shader* default_shader;
  Shader* default_shader_light;

  /////////////////
  // Scene Stuff
  //
  char *current_scene_name;
  char **scene_sequence;
  int current_scene_index;
  int number_of_scenes;

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

  GameMode game_mode;
  GameState game_state;
} Manager;

extern Manager* manager;

Manager* init_manager();

void Manager_set_active_camera(Manager *manager, int camera_index);
void Manager_add_camera(Manager *manager, Camera *camera);

void Manager_tick_timer(Manager* manager);

void Manager_add_entity(Manager *manager, Entity *entity);
void Manager_render_entities(Manager *manager);
void Manager_render_entities_with_shader(Manager *manager, Shader *shader);
void Manager_update_entities(Manager *manager);

void Manager_update_active_camera_location(Manager *manager);

Entity* Manager_get_first_entity_by_type(Manager *manager, EntityType type);
Entity* Manager_get_entity_by_type(Manager *manager, EntityType type, Entity *starting_entity);

Entity* Manager_entity_at_position(Manager *manager, vec3 position);
Entity* Manager_next_entity_at_position(Manager *manager, vec3 position, Entity *last_entity);
Entity* Manager_entity_at_position_of_type(Manager *manager, vec3 position, EntityType type);
int Manager_has_entity_at_position(Manager *manager, vec3 position);
int Manager_entity_at_position_count(Manager *manager, vec3 position);

void Manager_destroy_entity(Manager *manager, Entity *entity);
void Manager_destroy_entities(Manager *manager);
void Manager_destroy_camera(Manager *manager, Camera *camera);
void Manager_destroy_cameras(Manager *manager);
void Manager_destroy_lights(Manager *manager);

#endif
