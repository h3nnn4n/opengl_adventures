#ifndef _GRID_H
#define _GRID_H

#include "entity.h"
#include "manager.h"


void Grid_populate(Manager *manager);
void Grid_add(Grid *grid, Entity *entity);

//void make_key(vec3 value, vec3 *key);
Key make_key(vec3 value);

#endif
