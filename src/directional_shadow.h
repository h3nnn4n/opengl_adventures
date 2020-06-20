#ifndef _DIRECTIONAL_SHADOW_H
#define _DIRECTIONAL_SHADOW_H

#include "light.h"
#include "shader_c.h"

void build_directional_shadow_map(Light *light);
void render_directional_shadow_map(Light *light);
void bind_directional_shadow_map(Shader *shader, Light *light);

#endif /* _DIRECTIONAL_SHADOW_H */
