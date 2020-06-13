#include <assert.h>

#include <cJSON.h>

#include <stb.h>

#include "camera.h"
#include "manager.h"
#include "player.h"
#include "scene_save.h"

void save_scene(Manager *manager, char *scene_path){
  cJSON *json = cJSON_CreateObject();

  save_camera(manager, json);
  save_lights(manager, json);
  save_entities(manager, json);

  /*const char *json_string = cJSON_PrintUnformatted(json);*/
  char *json_string = cJSON_Print(json);
  printf(json_string);

  stb_filewritestr(scene_path, json_string);
}

void save_camera(Manager *manager, cJSON *json) {
  Camera *camera = manager->active_camera;

  cJSON *json_camera = cJSON_CreateObject();
  cJSON_AddItemToObject(json, "camera", json_camera);

  save_float(json_camera , "pitch" , camera->pitch );
  save_float(json_camera , "yaw"   , camera->yaw   );
  save_float(json_camera , "zoom"  , camera->zoom  );

  save_vec3(json_camera , "position" , camera->camera_pos    );
  save_vec3(json_camera , "front"    , camera->camera_front  );
  save_vec3(json_camera , "target"   , camera->camera_target );
  save_vec3(json_camera , "up"       , camera->camera_up     );
  save_vec3(json_camera , "right"    , camera->camera_right  );
}

void save_lights(Manager *manager, cJSON *json) {
  cJSON *json_lights = cJSON_AddArrayToObject(json, "lights");

  save_light(spot_light, json_lights);
  save_light(directional_light, json_lights);

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    save_light(point_lights[i], json_lights);
  }
}

void save_light(Light *light, cJSON *json) {
  if (light == NULL) return;

  cJSON *json_light = cJSON_CreateObject();

  switch (light->light_type) {
    case SPOTLIGHT:
      save_string(json_light, "type", "spotlight");

      save_float(json_light, "constant", light->constant);
      save_float(json_light, "linear", light->linear);
      save_float(json_light, "quadratic", light->quadratic);

      save_float(json_light, "cutOff", light->cutOff);
      save_float(json_light, "outterCutOff", light->outterCutOff);

      save_vec3(json_light, "position", light->position);
      save_vec3(json_light, "direction", light->direction);
      break;

    case POINT:
      save_string(json_light, "type", "point");

      save_float(json_light, "constant", light->constant);
      save_float(json_light, "linear", light->linear);
      save_float(json_light, "quadratic", light->quadratic);

      save_vec3(json_light, "position", light->position);
      break;

    case DIRECTIONAL:
      save_string(json_light, "type", "directional");

      save_vec3(json_light, "direction", light->direction);
      break;

    default:
      break;
  }

  save_int(json_light, "active", light->active);
  save_int(json_light, "draw", light->draw);

  save_vec3(json_light, "ambient", (float*)light->ambient);
  save_vec3(json_light, "diffuse", (float*)light->diffuse);
  save_vec3(json_light, "specular", (float*)light->specular);

  cJSON_AddItemToArray(json, json_light);
}

void save_entities(Manager *manager, cJSON *json) {
  cJSON *json_entities = cJSON_AddArrayToObject(json, "entities");

  for (int i = 0; i < manager->entity_count; ++i) {
    save_entity(manager->entities[i], json_entities);
  }
}

void save_entity(Entity *entity, cJSON *json) {
  if (entity == NULL) return;

  cJSON *json_entity = cJSON_CreateObject();

  save_int(json_entity, "type", entity->type);
  save_int(json_entity, "active", entity->active);
  save_int(json_entity, "deleted", entity->deleted);

  save_vec3(json_entity, "position", entity->position);
  save_vec3(json_entity, "scale", entity->scale);
  save_vec3(json_entity, "rotation", entity->rotation);

  save_string(json_entity, "model_path", entity->model_path);
  save_string(json_entity, "frag_shader_path", entity->frag_shader_path);
  save_string(json_entity, "vertex_shader_path", entity->vertex_shader_path);

  save_material(entity, json_entity);
  save_player(entity, json_entity);
  save_box(entity, json_entity);

  cJSON_AddItemToArray(json, json_entity);
}

void save_material(Entity *entity, cJSON *json) {
  cJSON *json_material = cJSON_CreateObject();
  cJSON_AddItemToObject(json, "material", json_material);

  save_vec3(json_material, "color", entity->color);
  save_float(json_material, "shininess", entity->shininess);
}

void save_player(Entity *entity, cJSON *json) {
  if (entity->type == PLAYER) {
    PlayerData *player_data = entity->data;
    cJSON *json_player_data = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "player_data", json_player_data);
    assert(player_data);

    save_int(json_player_data, "state", player_data->state);
    save_int(json_player_data, "move_direction", player_data->move_direction);

    save_vec3(json_player_data, "current_grid_pos", player_data->current_grid_pos);
    save_vec3(json_player_data, "moving_to_grid_pos", player_data->moving_to_grid_pos);

    save_float(json_player_data, "progress", player_data->progress);
    save_float(json_player_data, "move_speed", player_data->move_speed);
  }
}

void save_box(Entity *entity, cJSON *json) {
  if (entity->type != BOX) return;

  PlayerData *box_data = entity->data;
  cJSON *json_box_data = cJSON_CreateObject();
  cJSON_AddItemToObject(json, "box_data", json_box_data);
  assert(box_data);

  save_int(json_box_data, "state", box_data->state);
  save_int(json_box_data, "move_direction", box_data->move_direction);

  save_vec3(json_box_data, "current_grid_pos", box_data->current_grid_pos);
  save_vec3(json_box_data, "moving_to_grid_pos", box_data->moving_to_grid_pos);

  save_float(json_box_data, "progress", box_data->progress);
  save_float(json_box_data, "move_speed", box_data->move_speed);
}

void save_vec3(cJSON *json, const char* value_name, float* vec3) {
  assert(vec3);

  cJSON *json_vec3 = cJSON_CreateObject();
  cJSON_AddItemToObject(json, value_name, json_vec3);

  cJSON_AddNumberToObject(json_vec3, "x", vec3[0]);
  cJSON_AddNumberToObject(json_vec3, "y", vec3[1]);
  cJSON_AddNumberToObject(json_vec3, "z", vec3[2]);
}

void save_float(cJSON *json, const char* value_name, float value) {
  cJSON_AddNumberToObject(json, value_name, value);
}

void save_int(cJSON *json, const char* value_name, int value) {
  cJSON_AddNumberToObject(json, value_name, value);
}

void save_string(cJSON *json, const char* value_name, const char* value) {
  cJSON_AddStringToObject(json, value_name, value);
}
