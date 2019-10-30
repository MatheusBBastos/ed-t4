#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <stdbool.h>

#include "modules/sig/object.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"

bool compareObjectToId(void *obj, void *idVoid);

bool compareBlockToCep(void *block, void *cepVoid);

bool compareEquipToId(void *eq, void *idVoid);

#endif