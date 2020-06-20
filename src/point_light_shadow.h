#ifndef _POINT_LIGHT_SHADOW_H
#define _POINT_LIGHT_SHADOW_H

#include "light.h"

void build_pointlight_shadow_maps();
void render_pointlight_shadow_maps();

void build_pointlight_shadow_map(Light *light);
void render_pointlight_shadow_map(Light *light);

void bind_pointlight_shadow_map(Shader *shader, Light *light);

#endif /* _POINT_LIGHT_SHADOW_H */
