#include "comparators.h"

bool compareObjectToId(void *obj, void *idVoid) {
    char *id = (char *) idVoid;
    return (strcmp(Object_GetId(obj), id) == 0);
}

bool compareBlockToCep(void *block, void *cepVoid) {
    char *cep = (char *) cepVoid;
    return (strcmp(Block_GetCep(block), cep) == 0);
}

bool compareEquipToId(void *eq, void *idVoid) {
    char *id = (char *) idVoid;
    return (strcmp(Equip_GetID(eq), id) == 0);
}
