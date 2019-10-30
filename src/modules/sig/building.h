#ifndef BUILDING_H
#define BUILDING_H

#include <stdlib.h>
#include "../aux/point.h"
#include "../aux/segment.h"

typedef void *Building;

Building Building_Create(double x, double y, double w, double h, double num);

// Posiciona quatro ou seis segmentos no vetor 'vector' a partir do prédio informado,
// levando em conta se os novos segmentos seriam cortados pela semirreta partindo
// das coordenadas de origem e com direção horizontal à esquerda
Segment *Building_PutSegments(Building buildingVoid, Segment *vector, double x, double y);

double Building_GetX(Building building);

double Building_GetY(Building building);

double Building_GetW(Building building);

double Building_GetH(Building building);

double Building_GetNum(Building building);

void Building_SetX(Building building, double x);

void Building_SetY(Building building, double y);

void Building_SetW(Building building, double w);

void Building_SetH(Building building, double h);

void Building_SetNum(Building building, double num);

void Building_Destroy(Building building);

#endif
