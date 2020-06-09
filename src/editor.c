#include <math.h>

#include "editor.h"
#include "entity.h"

SelectedEntity selected_entity;

void snap_to_grid(Entity *entity) {
  for (int i = 0; i < 3; i++) {
    entity->position[i] = round(entity->position[i]);
  }
}
