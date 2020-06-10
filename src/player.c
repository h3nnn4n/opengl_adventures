#include <assert.h>
#include <math.h>

#include "manager.h"
#include "player.h"

void move_player(Manager *manager, Direction direction) {
  Entity *entity = Manager_get_first_entity_by_type(manager, PLAYER);

  // No player exists, for whatever reason
  if (entity == NULL) return;

  assert(entity->data);

  PlayerData *data = entity->data;

  if (data->state != IDLE) return;

  switch (direction) {
    case LEFT:
      data->state = MOVING;
      data->progress = 1;

      data->current_grid_pos[0] = round(entity->position[0]);
      data->current_grid_pos[1] = round(entity->position[1]);
      data->current_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] = round(entity->position[0]);
      data->moving_to_grid_pos[1] = round(entity->position[1]);
      data->moving_to_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] -= 1;
      data->move_direction = direction;
      break;

    case RIGHT:
      data->state = MOVING;
      data->progress = 1;

      data->current_grid_pos[0] = round(entity->position[0]);
      data->current_grid_pos[1] = round(entity->position[1]);
      data->current_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] = round(entity->position[0]);
      data->moving_to_grid_pos[1] = round(entity->position[1]);
      data->moving_to_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] += 1;
      data->move_direction = direction;
      break;

    case FRONT:
      data->state = MOVING;
      data->progress = 1;

      data->current_grid_pos[0] = round(entity->position[0]);
      data->current_grid_pos[1] = round(entity->position[1]);
      data->current_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] = round(entity->position[0]);
      data->moving_to_grid_pos[1] = round(entity->position[1]);
      data->moving_to_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[2] += 1;
      data->move_direction = direction;
      break;

    case BACK:
      data->state = MOVING;
      data->progress = 1;

      data->current_grid_pos[0] = round(entity->position[0]);
      data->current_grid_pos[1] = round(entity->position[1]);
      data->current_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[0] = round(entity->position[0]);
      data->moving_to_grid_pos[1] = round(entity->position[1]);
      data->moving_to_grid_pos[2] = round(entity->position[2]);

      data->moving_to_grid_pos[2] -= 1;
      data->move_direction = direction;
      break;

    default:
      break;
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
