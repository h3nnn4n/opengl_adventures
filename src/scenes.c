#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <stb.h>

#include "manager.h"
#include "scene_loader.h"
#include "scenes.h"

void load_scene_sequence(Manager *manager, char *scene_sequence_path) {
  if (manager->scene_sequence != NULL) free(manager->scene_sequence);

  int len = -1;
  manager->scene_sequence = stb_stringfile(scene_sequence_path, &len);

  fprintf(stdout, "Loaded scene sequence with %d scenes\n", len);
}

void load_scene_number(Manager *manager, int index) {
  load_scene(manager, manager->scene_sequence[index]);
  manager->current_scene_index = index;
}

void load_next_scene(Manager *manager) {
  manager->current_scene_index += 1;
  load_scene(manager, manager->scene_sequence[manager->current_scene_index]);
}
