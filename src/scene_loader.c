#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <cJSON.h>

#include <stb.h>

#include "box.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "scene_loader.h"
#include "player.h"

void load_scene(Manager *manager, char *scene_path) {
  Manager_destroy_entities(manager);

  fprintf(stdout, "\n");

  if (scene_path == NULL) {
    scene_path = manager->current_scene_name;
  }

  size_t plen = -1;
  char *json_file = stb_file(scene_path, &plen);

  fprintf(stdout, "Loading scene %s of %ld bytes\n", scene_path, plen);

  cJSON *json = cJSON_Parse(json_file);

  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
      abort();
    }
  }

  memcpy(manager->current_scene_name, scene_path, strlen(scene_path));

  /*printf("yay\n");*/

  /*char *string = cJSON_Print(json);*/
  /*printf(string);*/
  /*printf("\n");*/

  // TODO: We need to free the objects from the old scene before loading the
  // new ones. aka stop leaking memory
  // This is a HACK to allow loading new stuff

  manager->camera_count = 0;
  manager->entity_count = 0;

  load_camera(manager, json);
  load_lights(manager, json);
  load_entities(manager, json);

  /*fprintf(stdout, "\n");*/

  cJSON_Delete(json);
}

void load_camera(Manager *manager, cJSON *json) {
  cJSON *json_camera = cJSON_GetObjectItemCaseSensitive(json, "camera");

  if (!cJSON_IsObject(json_camera)) {
    /*printf("  Camera settings not found\n");*/
    return;
  }

  /*printf("Camera settings found\n");*/

  Camera* camera = make_camera();
  Manager_add_camera(manager, camera);
  Manager_set_active_camera(manager, 0);

  load_float(json_camera , "pitch" , &camera->pitch );
  load_float(json_camera , "yaw"   , &camera->yaw   );
  load_float(json_camera , "zoom"  , &camera->zoom  );

  load_vec3(json_camera , "position" , camera->camera_pos    );
  load_vec3(json_camera , "front"    , camera->camera_front  );
  load_vec3(json_camera , "target"   , camera->camera_target );
  load_vec3(json_camera , "up"       , camera->camera_up     );
  load_vec3(json_camera , "right"    , camera->camera_right  );
}

void load_lights(Manager *manager, cJSON *json) {
  cJSON *json_lights = cJSON_GetObjectItemCaseSensitive(json, "lights");

  if (!cJSON_IsArray(json_lights)) {
    /*printf("  Lights settings not found\n");*/
    return;
  }

  cJSON *json_light;
  int point_light_index = 0;

  cJSON_ArrayForEach(json_light, json_lights) {
    cJSON *json_type = cJSON_GetObjectItemCaseSensitive(json_light, "type");

    if (!cJSON_IsString(json_type)) {
      continue;
    }

    LightType light_type = -1;

    if (strcmp(json_type->valuestring, "point") == 0) {
      light_type = POINT;
    } else if (strcmp(json_type->valuestring, "directional") == 0) {
      light_type = DIRECTIONAL;
    } else if (strcmp(json_type->valuestring, "spotlight") == 0) {
      light_type = SPOTLIGHT;
    }

    /*printf("Light settings found\n");*/

    Light *light = make_light(light_type);
    light->shader = manager->default_shader;

    load_vec3(json_light, "ambient", (float*)light->ambient);
    load_vec3(json_light, "diffuse", (float*)light->diffuse);
    load_vec3(json_light, "specular", (float*)light->specular);

    load_int(json_light, "active", &light->active);
    if (load_int(json_light, "draw", &light->draw)) {
      light->shader_light_obj = manager->default_shader_light;
      load_light_model(light, light->shader_light_obj, "assets/cube/cube.obj");
    }

    switch (light_type) {
      case POINT:
        load_vec3(json_light, "position", (float*)light->position);

        load_float(json_light, "constant", &light->constant);
        load_float(json_light, "linear", &light->linear);
        load_float(json_light, "quadratic", &light->quadratic);

        point_lights[point_light_index] = light;
        point_light_index++;
        break;

      case SPOTLIGHT:
        load_vec3(json_light, "position", (float*)light->position);
        load_vec3(json_light, "direction", (float*)light->direction);

        load_float(json_light, "constant", &light->constant);
        load_float(json_light, "linear", &light->linear);
        load_float(json_light, "quadratic", &light->quadratic);

        load_float(json_light, "cutOff", &light->cutOff);
        load_float(json_light, "outterCutOff", &light->outterCutOff);

        spot_light = light;
        break;

      case DIRECTIONAL:
        load_vec3(json_light, "direction", (float*)light->direction);

        directional_light = light;
        break;

      default:
        break;
    }
  }
}

void load_entities(Manager *manager, cJSON *json) {
  cJSON *json_entities = cJSON_GetObjectItemCaseSensitive(json, "entities");

  if (!cJSON_IsArray(json_entities)) {
    /*printf("  Entities array not found\n");*/
    return;
  }

  /*printf("Entities found\n");*/

  cJSON *json_entity;

  cJSON_ArrayForEach(json_entity, json_entities) {
    Entity *entity = new_entity();
    entity->shader = manager->default_shader;

    load_int(json_entity, "type", (int*)&entity->type);
    load_int(json_entity, "active", &entity->active);
    load_int(json_entity, "deleted", &entity->deleted);

    load_vec3(json_entity, "position", (float*)entity->position);
    load_vec3(json_entity, "scale", (float*)entity->scale);
    load_vec3(json_entity, "rotation", (float*)entity->rotation);

    load_string(json_entity, "model_path", &entity->model_path);
    load_string(json_entity, "frag_shader_path", &entity->frag_shader_path);
    load_string(json_entity, "vertex_shader_path", &entity->vertex_shader_path);

    load_model(entity, entity->model_path);
    load_material (json_entity, entity);
    load_player_data(json_entity, entity);
    load_box_data(json_entity, entity);

    Manager_add_entity(manager, entity);
  }
}

void load_material (cJSON *json_entity, Entity *entity) {
  cJSON *json_material = cJSON_GetObjectItemCaseSensitive(json_entity, "material");

  load_float(json_material, "shininess", (float*)&entity->shininess);
  load_vec3(json_material, "color", (float*)entity->color);
}

void load_player_data(cJSON *json_entity, Entity *entity) {
  if (entity->type != PLAYER) return;

  cJSON *json_player_data = cJSON_GetObjectItemCaseSensitive(json_entity, "player_data");
  PlayerData *player_data = malloc(sizeof(PlayerData));
  assert(player_data);
  entity->data = player_data;

  load_int(json_player_data, "state", (int*)&player_data->state);
  assert(player_data->state == IDLE || player_data->state == MOVING);

  load_int(json_player_data, "move_direction", (int*)&player_data->move_direction);

  load_vec3(json_player_data, "current_grid_pos", (float*)player_data->current_grid_pos);
  load_vec3(json_player_data, "moving_to_grid_pos", (float*)player_data->moving_to_grid_pos);

  load_float(json_player_data, "progress", (float*)&player_data->progress);
  load_float(json_player_data, "move_speed", (float*)&player_data->move_speed);
}

void load_box_data(cJSON *json_entity, Entity *entity) {
  if (entity->type != BOX) return;

  cJSON *json_box_data = cJSON_GetObjectItemCaseSensitive(json_entity, "box_data");
  BoxData *box_data = malloc(sizeof(BoxData));
  assert(box_data);
  entity->data = box_data;

  load_int(json_box_data, "state", (int*)&box_data->state);
  assert(box_data->state == STOPPED || box_data->state == BEING_PUSHED);

  load_int(json_box_data, "move_direction", (int*)&box_data->move_direction);

  load_vec3(json_box_data, "current_grid_pos", (float*)box_data->current_grid_pos);
  load_vec3(json_box_data, "moving_to_grid_pos", (float*)box_data->moving_to_grid_pos);

  load_float(json_box_data, "progress", (float*)&box_data->progress);
  load_float(json_box_data, "move_speed", (float*)&box_data->move_speed);
}

int load_string(cJSON *json, const char* value_name, char** value) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (cJSON_IsString(json_value)) {
    int len = sizeof(char) * (strlen(json_value->valuestring) + 1);

    *value = malloc(len);
    memcpy(*value, json_value->valuestring, len);

    /*printf("  %s found: %s\n",*/
      /*value_name,*/
      /**value*/
    /*);*/

    return 1;
  }

  /*printf("  %s was not found\n", value_name);*/
  return 0;
}

int load_float(cJSON *json, const char* value_name, float* value) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (cJSON_IsNumber(json_value)) {
    value[0] = json_value->valuedouble;

    /*printf("  %s found: %4.2f\n",*/
      /*value_name,*/
      /*value[0]*/
    /*);*/

    return 1;
  }

  /*printf("  %s was not found\n", value_name);*/
  return 0;
}

int load_int(cJSON *json, const char* value_name, int* value) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (cJSON_IsNumber(json_value)) {
    value[0] = json_value->valueint;

    /*printf("  %s found: %d\n",*/
      /*value_name,*/
      /*value[0]*/
    /*);*/

    return 1;
  }

  /*printf("  %s was not found\n", value_name);*/
  return 0;
}

int load_vec3(cJSON *json, const char* value_name, float* vec3) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (!cJSON_IsObject(json_value)) {
    /*printf("  %s was not found\n", value_name);*/
    return 0;
  }

  cJSON *json_x = cJSON_GetObjectItemCaseSensitive(json_value, "x");
  cJSON *json_y = cJSON_GetObjectItemCaseSensitive(json_value, "y");
  cJSON *json_z = cJSON_GetObjectItemCaseSensitive(json_value, "z");

  float x = 0;
  float y = 0;
  float z = 0;

  if (cJSON_IsNumber(json_x)) {
    x = json_x->valuedouble;
  }

  if (cJSON_IsNumber(json_y)) {
    y = json_y->valuedouble;
  }

  if (cJSON_IsNumber(json_z)) {
    z = json_z->valuedouble;
  }

  vec3[0] = x;
  vec3[1] = y;
  vec3[2] = z;

  /*printf("  %s found: %4.2f %4.2f %4.2f\n",*/
    /*value_name,*/
    /*vec3[0],*/
    /*vec3[1],*/
    /*vec3[2]*/
  /*);*/

  return 1;
}
