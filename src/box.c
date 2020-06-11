#include <assert.h>

#include "box.h"
#include "entity.h"

void box_update(Entity* box) {
  assert(box->data);

  BoxData *box_data = box->data;

  switch (box_data->state) {
    case BEING_PUSHED:
      if (box_data->progress > 0) {
        update_box_position(box);
      } else {
        glm_vec3_copy(box_data->moving_to_grid_pos, box->position);
        box_data->state = STOPPED;
      }
      break;
    case STOPPED:
      break;
    case BOX_FALLING:
      // TODO
      break;
    default:
      assert(false); // fail loudly and soon
      break;
  }
}

void update_box_position(Entity *box) {
  BoxData *box_data = box->data;

  box_data->progress -= manager->delta_time * box_data->move_speed;

  glm_vec3_lerp(
    box_data->current_grid_pos,
    box_data->moving_to_grid_pos,
    1 - box_data->progress,
    box->position
  );
}
