#ifndef _LIGHT_H
#define _LIGHT_H

#include <cglm/cglm.h>

#include "shader_c.h"
#include "model_c.h"

#define NR_POINT_LIGHTS 4

typedef enum {
  SPOTLIGHT,
  DIRECTIONAL,
  POINT,
} LightType;

typedef struct {
  mat4 shadow_projection;
  mat4 shadow_view; // Used for directional lights
  mat4 light_space_matrix;
  mat4 shadow_views[6]; // Used for pointlights lights
  mat4 light_space_matrixes[6];

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
  Shader *shader_light_obj;

  unsigned int depthMapFBO;
  unsigned int depthMap;

  int active;
  int draw;

  Model *model;
} Light;

extern Light *directional_light;
extern Light *spot_light;
extern Light *point_lights[];

Light* make_light(LightType type);

void destroy_light(Light *light);
void refresh_lights();
void refresh_light(Light *light);

void draw_point_lights();
void load_light_model(Light *light, Shader *shader, char* model_path);

void set_position(Light *light, float* value);
void set_direction(Light *light, float* value);
void set_ambient(Light *light, float* value);
void set_specular(Light *light, float* value);
void set_diffuse(Light *light, float* value);

#endif
