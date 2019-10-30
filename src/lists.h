#ifndef LISTS_H
#define LISTS_H

#include "modules/data_structures/static_list.h"
#include "modules/sig/object.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"
#include "modules/sig/building.h"
#include "modules/sig/wall.h"

#define DEFAULT_MAXIMUM 1000

void initializeLists();

void resizeLists(int nx, int nb, int nh, int nc, int nt, int np, int nw);

void destroyLists();

StList getObjList();

StList getBlockList();

StList getHydList();

StList getCTowerList();

StList getTLightList();

StList getBuildingList();

StList getWallList();

#endif