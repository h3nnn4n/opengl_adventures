#ifndef _LIGHT_H
#define _LIGHT_H

#include <cglm/cglm.h>

#include "shader_c.h"

#define NR_POINT_LIGHTS 4

typedef enum {
  SPOTLIGHT,
  DIRECTIONAL,
  POINT,
} LightType;

typedef struct {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float cutOff;
  float outterCutOff;

  LightType light_type;
  int point_light_index;

  Shader *shader;

  int active;
} Light;

extern Light *directional_light;
extern Light *spot_light;
extern Light *point_lights[];

Light* make_light(LightType type);

void destroy_light(Light *light);
void refresh_lights();
void refresh_light(Light *light);

void set_position(Light *light, float* value);
void set_direction(Light *light, float* value);
void set_ambient(Light *light, float* value);
void set_specular(Light *light, float* value);
void set_diffuse(Light *light, float* value);

#endif
