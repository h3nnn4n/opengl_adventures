#ifndef _SCENE_SAVE_H
#define _SCENE_SAVE_H

#include <cJSON.h>

#include "camera.h"
#include "light.h"
#include "manager.h"

void save_scene(Manager *manager);
void save_camera(Manager *manager, cJSON *json);
void save_lights(Manager *manager, cJSON *json);
void save_light(Light *light, cJSON *json);
void save_entities(Manager *manager, cJSON *json);
void save_entity(Entity *entity, cJSON *json);

void save_vec3(cJSON *json, const char* value_name, float *vec3);
void save_float(cJSON *json, const char* value_name, float value);
void save_int(cJSON *json, const char* value_name, int value);
void save_string(cJSON *json, const char* value_name, const char* value);

#endif
