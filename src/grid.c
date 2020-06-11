#include <math.h>

#include "entity.h"
#include "grid.h"
#include "manager.h"

#include "stb_ds.h"

void Grid_populate(Manager *manager) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    if (entity == NULL) continue;
    if (entity->type != BLOCK) continue;

    Grid_add(manager->grid, entity);
  }
}

void Grid_add(Grid *grid, Entity *entity) {
  Key key = make_key(entity->position);
  Entity *p = 0x123;
  Hash *hash = NULL;
  /*hmput(grid->hash, key, entity);*/
  hmput(hash, key, p);
  /*hmputs(grid->hash, key);*/
}

Key make_key(vec3 value) {
  Key key = {
    round(value[0]),
    round(value[1]),
    round(value[2])
  };

  /*for (int i = 0; i < 3; ++i) {*/
    /*key[i] = round(value[i]);*/
  /*}*/

  return key;
}
