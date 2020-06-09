#ifndef _EDITOR_H
#define _EDITOR_H

#include "entity.h"

extern Entity *selected_entity;

void snap_to_grid(Entity *entity);

#endif
