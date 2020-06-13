#include <assert.h>

#include "game.h"
#include "manager.h"
#include "scenes.h"

void game_logic(Manager *manager) {
  check_scene_win_conditions(manager);

  if (manager->game_state == WON) {
    load_next_scene(manager);
  }
}

void check_scene_win_conditions(Manager *manager) {
  assert(manager);
  bool all_targets_checked = true;

  for (Entity *box = Manager_get_first_entity_by_type(manager, BOX);
       box != NULL;
       box = Manager_get_entity_by_type(manager, BOX, box)) {

    bool checked = false;

    for (Entity *target = Manager_get_first_entity_by_type(manager, TARGET);
         target != NULL;
         target = Manager_get_entity_by_type(manager, TARGET, target)) {

      if (target && glm_vec3_eqv_eps(box->position, target->position)) {
        printf("win\n");
        checked = true;
        break;
      }
    }

    if (!checked) {
      all_targets_checked = false;
      break;
    }
  }

  if (all_targets_checked) {
    manager->game_state = WON;
  }
}
