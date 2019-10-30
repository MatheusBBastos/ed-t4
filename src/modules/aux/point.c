#include "point.h"

typedef struct point_t {
    double x;
    double y;
    double angle;
    double distance;
    bool starting;
    Segment segment;
} *PointPtr;

Point Point_Create(double x, double y, Segment segment, double angle, double distance) {
    PointPtr point = malloc(sizeof(struct point_t));
    point->x = x;
    point->y = y;
    point->angle = angle;
    point->distance = distance;
    point->starting = false;
    point->segment = segment;
    return point;
}

int Point_Compare(const void *p1Void, const void *p2Void) {
    PointPtr p1 = (PointPtr) *((Point*) p1Void);
    PointPtr p2 = (PointPtr) *((Point*) p2Void);

    if (p1->angle < p2->angle)
        return -1;
    else if (p1->angle > p2->angle)
        return 1;
    else if (p1->distance < p2->distance)
        return -1;
    else if (p1->distance > p2->distance)
        return 1;
    else if (p1->starting && !p2->starting)
        return 1;
    else if (!p1->starting && p2->starting)
        return -1;
    else if (Segment_GetDistance(p1->segment) < Segment_GetDistance(p2->segment))
        return 1;
    else if (Segment_GetDistance(p1->segment) > Segment_GetDistance(p2->segment))
        return -1;
    else
        return 0;
}

double Point_GetX(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->x;
}

double Point_GetY(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->y;
}

double Point_GetAngle(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->angle;
}

double Point_GetDistance(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->distance;
}

bool Point_IsStarting(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->starting;
}

Segment Point_GetSegment(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->segment;
}

void Point_SetX(Point pointVoid, double x) {
    PointPtr point = (PointPtr) pointVoid;
    point->x = x;
}

void Point_SetY(Point pointVoid, double y) {
    PointPtr point = (PointPtr) pointVoid;
    point->y = y;
}

void Point_SetAngle(Point pointVoid, double angle) {
    PointPtr point = (PointPtr) pointVoid;
    point->angle = angle;
}

void Point_SetDistance(Point pointVoid, double distance) {
    PointPtr point = (PointPtr) pointVoid;
    point->distance = distance;
}

void Point_SetStarting(Point pointVoid, bool starting) {
    PointPtr point = (PointPtr) pointVoid;
    point->starting = starting;
}

void Point_SetSegment(Point pointVoid, Segment segment) {
    PointPtr point = (PointPtr) pointVoid;
    point->segment = segment;
}

void Point_Destroy(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    free(point);
}
