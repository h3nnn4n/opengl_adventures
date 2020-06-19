#ifndef _DIRECTIONAL_SHADOW_H
#define _DIRECTIONAL_SHADOW_H

#include "shader_c.h"

void build_directional_shadow_map();
void render_directional_shadow_map();
void bind_directional_shadow_map(Shader *shader);

#endif /* _DIRECTIONAL_SHADOW_H */
