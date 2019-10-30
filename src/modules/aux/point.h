#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <stdbool.h>
#include "segment.h"
#include "../sig/geometry.h"

typedef void *Segment;

typedef void *Point;

Point Point_Create(double x, double y, Segment segment, double angle, double distance);

// Compara dois pontos para determinar qual é anterior. Para ser usado na função qsort
int Point_Compare(const void *p1, const void *p2);

double Point_GetX(Point pointVoid);

double Point_GetY(Point pointVoid);

double Point_GetAngle(Point pointVoid);

double Point_GetDistance(Point pointVoid);

bool Point_IsStarting(Point pointVoid);

Segment Point_GetSegment(Point pointVoid);

void Point_SetX(Point pointVoid, double x);

void Point_SetY(Point pointVoid, double y);

void Point_SetAngle(Point pointVoid, double angle);

void Point_SetDistance(Point pointVoid, double distance);

void Point_SetStarting(Point pointVoid, bool starting);

void Point_SetSegment(Point pointVoid, Segment segment);

void Point_Destroy(Point pointVoid);

#endif
