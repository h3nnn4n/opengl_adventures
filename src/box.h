#ifndef _BOX_H
#define _BOX_H

#include <cglm/cglm.h>

#include "manager.h"
#include "utils.h"

typedef enum {
  STOPPED,
  BEING_PUSHED,
  BOX_FALLING
} BoxState;

typedef struct {
  BoxState state;

  Direction move_direction;

  vec3 current_grid_pos;
  vec3 moving_to_grid_pos;

  float progress;
  float move_speed;
} BoxData;

void box_update(Entity* box);
void update_box_position(Entity *box);

#endif
