#ifndef _SCENES_H
#define _SCENES_H

#include "manager.h"

void load_scene_sequence(Manager *manager, char *scene_sequence_path);
void load_next_scene(Manager *manager);
void load_scene_number(Manager *manager, int index);

#endif
