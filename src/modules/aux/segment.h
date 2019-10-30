#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdlib.h>
#include <math.h>
#include "point.h"
#include "../sig/geometry.h"

typedef void *Point;

typedef void *Segment;

Segment Segment_Create(Point pStart, Point pEnd);

// Cria um segmento a partir das coordenadas informadas, já criando seus pontos e determinando
// qual é inicial, levando em conta as coordenadas de origem do raio de luz
Segment Segment_CreateFromCoords(double x1, double y1, double x2, double y2, double xSource, double ySource);

// Verifica em qual valor de 'x' o segmento intersecta com a semirreta horizontal à esquerda
// em y, retornando -1 caso não haja uma intersecção
double Segment_CheckXIntersection(Segment segmentVoid, double y);

// Corta o segmento em dois nas coordenadas (xInter, ySource), posicionando-os no vetor 'vector'
Segment *Segment_Cut(Segment segment, Segment *vector, double xInter, double xSource, double ySource);

Point Segment_GetPStart(Segment segmentVoid);

Point Segment_GetPEnd(Segment segmentVoid);

double Segment_GetXBiombo(Segment segmentVoid);

double Segment_GetYBiombo(Segment segmentVoid);

double Segment_GetDistance(Segment segmentVoid);

void Segment_SetPStart(Segment segmentVoid, Point pStart);

void Segment_SetPEnd(Segment segmentVoid, Point pEnd);

void Segment_SetXBiombo(Segment segmentVoid, double xBiombo);

void Segment_SetYBiombo(Segment segmentVoid, double yBiombo);

void Segment_SetDistance(Segment segmentVoid, double distance);

void Segment_Destroy(Segment segmentVoid);

#endif
