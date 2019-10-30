#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdbool.h>
#include <math.h>
#include "object.h"
#include "circle.h"
#include "rectangle.h"

#define PI 3.1415192653589793

double euclideanDistance(double x1, double y1, double x2, double y2);

double manhattanDistance(double x1, double y1, double x2, double y2);

double clamp(double value, double a, double b);

bool checkOverlap(Object a, Object b);

bool checkInside(Object obj, double x, double y);

void getCenter(Object obj, double *x, double *y);

void getSurroundingRect(Object obj, Rectangle resultRect);

double max(double a, double b);

double min(double a, double b);

double calculateAngle(double y, double x);

#endif