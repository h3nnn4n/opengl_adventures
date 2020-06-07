#include <cJSON.h>

#include "camera.h"
#include "manager.h"
#include "scene_save.h"
#include "stb.h"

void save_scene(Manager *manager){
  cJSON *json = cJSON_CreateObject();

  save_camera(manager, json);
  save_lights(manager, json);

  /*const char *json_string = cJSON_PrintUnformatted(json);*/
  char *json_string = cJSON_Print(json);
  printf(json_string);

  stb_filewritestr("scenes/test_scene.json", json_string);
}

void save_camera(Manager *manager, cJSON *json) {
  Camera *camera = manager->active_camera;

  cJSON *json_camera = cJSON_CreateObject();
  cJSON_AddItemToObject(json, "camera", json_camera);

  save_vec3(json_camera, "position", camera->camera_pos);
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

void save_vec3(cJSON *json, const char* value_name, float* vec3) {
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
