#include <stdio.h>
#include <math.h>

#include <cglm/cglm.h>

#include "editor.h"
#include "entity.h"
#include "manager.h"

SelectedEntity selected_entity;

void snap_to_grid(Entity *entity) {
  for (int i = 0; i < 3; i++) {
    entity->position[i] = round(entity->position[i]);
  }
}

void add_new_block(Manager *manager) {
  vec3 position_to_add;
  glm_vec3_copy(selected_entity.entity->position, position_to_add);
  glm_vec3_add(position_to_add, selected_entity.normal, position_to_add);

  // TODO: We should have a catalog or something like that for instantiating new objects
  Entity *entity = new_entity();
  glm_vec3_copy(position_to_add, entity->position);
  entity->type = BLOCK;
  entity->shader = manager->default_shader;
  load_model(entity, "assets/cube/cube.obj");

  Manager_add_entity(manager, entity);

  selected_entity.entity = NULL;
}
