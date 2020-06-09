#ifndef _EDITOR_H
#define _EDITOR_H

#include "entity.h"

typedef struct {
  Entity *entity;
  vec3 normal;
  vec2 screen_click_position;
} SelectedEntity;

extern SelectedEntity selected_entity;

void snap_to_grid(Entity *entity);

#endif
