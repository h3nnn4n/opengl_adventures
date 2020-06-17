#include <assert.h>
#include <stdio.h>
#include <math.h>

#include <cglm/cglm.h>

#include "box.h"
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

void clear_blocks(Manager *manager) {
  for (int entity_index = 0; entity_index < manager->entity_count; ++entity_index) {
    Entity *entity = manager->entities[entity_index];

    if (entity == NULL) continue;
    if (entity->type != BLOCK) continue;

    entity->deleted = 1;
    manager->entities[entity_index] = NULL;

    // This causes more trouble than it is worth, so it is ok to leak memory in the editor
    /*destroy_entity(entity);*/
  }

  vec3 position_to_add = GLM_VEC3_ZERO_INIT;

  Entity *entity = new_entity();
  glm_vec3_copy(position_to_add, entity->position);
  entity->type = BLOCK;
  entity->shader = manager->default_shader;
  load_model(entity, "assets/cube/cube.obj");

  Manager_add_entity(manager, entity);
}

void change_entity_type_type(Entity *entity, EntityType type) {
  printf("%d %d\n", entity->type, type);

  if (entity->type == type) return;

  entity->type = type;

  switch (type) {
    case BOX:
      if (entity->data != NULL) free(entity->data);

      BoxData *box_data = malloc(sizeof(BoxData));
      assert(box_data);
      entity->data = box_data;

      box_data->state = STOPPED;
      box_data->move_direction = LEFT;

      break;

    case BLOCK:
      if (entity->data != NULL) free(entity->data);
      break;

    case TARGET:
      if (entity->data != NULL) free(entity->data);
      break;

    case PLAYER:
      assert(false); // TODO
      break;
  }
}
