#include <assert.h>
#include <math.h>

#include "manager.h"
#include "player.h"

void init_move(Entity *entity) {
  PlayerData *data = entity->data;

  data->state = MOVING;
  data->progress = 1;

  data->current_grid_pos[0] = round(entity->position[0]);
  data->current_grid_pos[1] = round(entity->position[1]);
  data->current_grid_pos[2] = round(entity->position[2]);

  data->moving_to_grid_pos[0] = round(entity->position[0]);
  data->moving_to_grid_pos[1] = round(entity->position[1]);
  data->moving_to_grid_pos[2] = round(entity->position[2]);
}

void move_player(Manager *manager, Direction direction) {
  Entity *entity = Manager_get_first_entity_by_type(manager, PLAYER);

  // No player exists, for whatever reason
  if (entity == NULL) return;

  assert(entity->data);

  PlayerData *data = entity->data;

  if (data->state != IDLE) return;

  init_move(entity);
  data->move_direction = direction;

  switch (direction) {
    case LEFT:
      data->moving_to_grid_pos[0] -= 1;
      break;

    case RIGHT:
      data->moving_to_grid_pos[0] += 1;
      break;

    case FRONT:
      data->moving_to_grid_pos[2] -= 1;
      break;

    case BACK:
      data->moving_to_grid_pos[2] += 1;
      break;

    default:
      assert(false); // fail loudly and soon
      break;
  }

  int cant_move = Manager_has_entity_at_position(manager, data->moving_to_grid_pos);

  if (cant_move) {
    data->state = IDLE;
  }
}

void player_update(Entity *player) {
  assert(player->data);

  PlayerData *pdata = player->data;

  switch (pdata->state) {
    case MOVING:
      if (pdata->progress > 0) {
        pdata->progress -= manager->delta_time * pdata->move_speed;

        glm_vec3_lerp(
          pdata->current_grid_pos,
          pdata->moving_to_grid_pos,
          1 - pdata->progress,
          player->position
        );
      } else {
        glm_vec3_copy(pdata->moving_to_grid_pos, player->position);
        pdata->state = IDLE;
      }
      break;
    case IDLE:
      break;
  }
}
