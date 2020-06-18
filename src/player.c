#include <assert.h>
#include <math.h>

#include <cglm/cglm.h>

#include "box.h"
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

  vec3 offset = GLM_VEC3_ZERO_INIT;

  switch (direction) {
    case LEFT:
      offset[0] -= 1;
      break;

    case RIGHT:
      offset[0] += 1;
      break;

    case FRONT:
      offset[2] -= 1;
      break;

    case BACK:
      offset[2] += 1;
      break;

    default:
      assert(false); // fail loudly and soon
      break;
  }

  glm_vec3_add(data->moving_to_grid_pos, offset, data->moving_to_grid_pos);

  Entity *obstacle = Manager_entity_at_position_of_type(manager, data->moving_to_grid_pos, BOX);

  if (obstacle == NULL)
    obstacle = Manager_entity_at_position_of_type(manager, data->moving_to_grid_pos, BLOCK);

  int cant_move = (
    obstacle != NULL && (
      obstacle->type == BLOCK || (
        obstacle->type == BOX &&
        !can_push_entity(manager, obstacle, direction)
      )
    )
  );

  if (cant_move) {
    data->state = IDLE;
  } else {
    if (obstacle != NULL && obstacle->type == BOX && can_push_entity(manager, obstacle, direction)) {
      BoxData *bdata = obstacle->data;

      bdata->state = BEING_PUSHED;
      bdata->progress = 1;
      bdata->move_speed = data->move_speed;

      bdata->current_grid_pos[0] = round(obstacle->position[0]);
      bdata->current_grid_pos[1] = round(obstacle->position[1]);
      bdata->current_grid_pos[2] = round(obstacle->position[2]);

      bdata->moving_to_grid_pos[0] = round(obstacle->position[0]);
      bdata->moving_to_grid_pos[1] = round(obstacle->position[1]);
      bdata->moving_to_grid_pos[2] = round(obstacle->position[2]);

      glm_vec3_add(bdata->moving_to_grid_pos, offset, bdata->moving_to_grid_pos);
    }
  }
}

int can_push_entity(Manager *manager, Entity *entity, Direction push_direction) {
  if (entity->type != BOX) return 0;

  vec3 push_position = { entity->position[0],
                         entity->position[1],
                         entity->position[2] };

  switch (push_direction) {
    case LEFT:
      push_position[0] -= 1;
      break;

    case RIGHT:
      push_position[0] += 1;
      break;

    case FRONT:
      push_position[2] -= 1;
      break;

    case BACK:
      push_position[2] += 1;
      break;

    default:
      assert(false); // fail loudly and soon
      break;
  }

  Entity *obstacle = Manager_entity_at_position(manager, push_position);

  int can_be_pushed = (
    obstacle == NULL || (
      obstacle->type == TARGET
    )
  );

  return can_be_pushed;
}

void update_position(Entity *player) {
  PlayerData *pdata = player->data;

  pdata->progress -= manager->delta_time * pdata->move_speed;

  glm_vec3_lerp(
    pdata->current_grid_pos,
    pdata->moving_to_grid_pos,
    1 - pdata->progress,
    player->position
  );
}

void player_update(Entity *player) {
  assert(player->data);

  PlayerData *pdata = player->data;

  switch (pdata->state) {
    case MOVING:
      if (pdata->progress > 0) {
        update_position(player);
      } else {
        glm_vec3_copy(pdata->moving_to_grid_pos, player->position);
        pdata->state = IDLE;
      }
      break;
    case IDLE:
      {
        vec3 position_below = { player->position[0],
                                player->position[1] - 1,
                                player->position[2] };

        int has_support = Manager_has_entity_at_position(manager, position_below);

        if (!has_support) {
          init_move(player);
          pdata->move_direction = DOWN;
          pdata->moving_to_grid_pos[1] -= 1;
          pdata->state = FALLING;
        }
      }
      break;
    case FALLING:
      if (pdata->progress > 0) {
        update_position(player);
      } else {
        glm_vec3_copy(pdata->moving_to_grid_pos, player->position);

        int cant_move = Manager_has_entity_at_position(manager, pdata->moving_to_grid_pos);
        if (cant_move) {
          pdata->state = IDLE;
        }
      }
      break;
    default:
      assert(false); // fail loudly and soon
      break;
  }
}
