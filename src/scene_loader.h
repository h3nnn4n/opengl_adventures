#ifndef _SCENE_LOADER_H
#define _SCENE_LOADER_H

#include <cJSON.h>

#include "manager.h"

void load_scene(Manager *manager, char *scene_path);
void load_camera(Manager *manager, cJSON *json);
void load_lights(Manager *manager, cJSON *json);
void load_entities(Manager *manager, cJSON *json);

int load_string(cJSON *json, const char* value_name, char** value);
int load_vec3(cJSON *json, const char* value_name, float* vec3);
int load_float(cJSON *json, const char* value_name, float* value);
int load_int(cJSON *json, const char* value_name, int* value);

#endif
