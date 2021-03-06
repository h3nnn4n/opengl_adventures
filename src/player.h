#ifndef _PLAYER_H
#define _PLAYER_H

#include <cglm/cglm.h>

#include "manager.h"
#include "utils.h"

typedef enum {
  IDLE,
  MOVING,
  FALLING
} PlayerState;

typedef struct {
  PlayerState state;

  Direction move_direction;

  vec3 current_grid_pos;
  vec3 moving_to_grid_pos;

  float progress;
  float move_speed;
} PlayerData;

void move_player(Manager *manager, Direction direction);
void player_update(Entity *player);

int can_push_entity(Manager *manager, Entity *entity, Direction push_direction);

#endif
