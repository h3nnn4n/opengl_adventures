#include <stdio.h>
#include <string.h>

#include <cJSON.h>

#include "camera.h"
#include "light.h"
#include "manager.h"
#include "scene_loader.h"
#include "stb.h"

void load_scene(Manager *manager) {
  fprintf(stdout, "\n");

  size_t plen = -1;
  char *scene_path = "scenes/test.json";
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

  /*printf("yay\n");*/

  /*char *string = cJSON_Print(json);*/
  /*printf(string);*/
  /*printf("\n");*/

  load_camera(manager, json);
  load_lights(manager, json);

  fprintf(stdout, "\n");

  cJSON_Delete(json);
}

void load_camera(Manager *manager, cJSON *json) {
  cJSON *json_camera = cJSON_GetObjectItemCaseSensitive(json, "camera");

  if (!cJSON_IsObject(json_camera)) {
    printf("  Camera settings not found\n");
    return;
  }

  printf("Camera settings found\n");

  Camera* camera = make_camera();
  Manager_add_camera(manager, camera);
  Manager_set_active_camera(manager, 0);


  load_float(json_camera, "pitch", &camera->pitch);
  load_float(json_camera, "yaw", &camera->yaw);

  load_vec3(json_camera, "position", (float*)camera->camera_pos);

  if (load_vec3(json_camera, "target", (float*)camera->camera_front)) {
    glm_vec3_normalize(camera->camera_front);
  }
}

void load_lights(Manager *manager, cJSON *json) {
  cJSON *json_lights = cJSON_GetObjectItemCaseSensitive(json, "lights");

  if (!cJSON_IsArray(json_lights)) {
    printf("  Lights settings not found\n");
    return;
  }

  cJSON *json_light;
  int point_light_index = 0;

  cJSON_ArrayForEach(json_light, json_lights) {
    cJSON *json_type = cJSON_GetObjectItemCaseSensitive(json_light, "type");

    if (!cJSON_IsString(json_type)) {
      continue;
    }

    LightType light_type = UNKNOWN_LIGHT;

    if (strcmp(json_type->valuestring, "point") == 0) {
      light_type = POINT;
    } else if (strcmp(json_type->valuestring, "directional") == 0) {
      light_type = DIRECTIONAL;
    } else if (strcmp(json_type->valuestring, "spotlight") == 0) {
      light_type = SPOTLIGHT;
    }

    printf("Light settings found\n");

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

int load_float(cJSON *json, const char* value_name, float* value) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (cJSON_IsNumber(json_value)) {
    value[0] = json_value->valuedouble;

    printf("  %s found: %4.2f\n",
      value_name,
      value[0]
    );

    return 1;
  }

  printf("  %s was not found\n", value_name);
  return 0;
}

int load_int(cJSON *json, const char* value_name, int* value) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (cJSON_IsNumber(json_value)) {
    value[0] = json_value->valueint;

    printf("  %s found: %d\n",
      value_name,
      value[0]
    );

    return 1;
  }

  printf("  %s was not found\n", value_name);
  return 0;
}

int load_vec3(cJSON *json, const char* value_name, float* vec3) {
  cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json, value_name);

  if (!cJSON_IsObject(json_value)) {
    printf("  %s was not found\n", value_name);
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

  printf("  %s found: %4.2f %4.2f %4.2f\n",
    value_name,
    vec3[0],
    vec3[1],
    vec3[2]
  );

  return 1;
}
