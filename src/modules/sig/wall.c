#include "wall.h"

#include <stdlib.h>

typedef struct wall_t {
    double x1;
    double y1;
    double x2;
    double y2;
} *WallPtr;

Wall Wall_Create(double x1, double y1, double x2, double y2) {
    WallPtr wall = malloc(sizeof(struct wall_t));
    wall->x1 = x1;
    wall->y1 = y1;
    wall->x2 = x2;
    wall->y2 = y2;
    return wall;
}

Segment *Wall_PutSegments(Wall wallVoid, Segment *vector, double xSource, double ySource) {
    WallPtr wall = (WallPtr) wallVoid;

    if (wall->x1 == wall->x2 && wall->x1 == xSource || wall->y1 == wall->y2 && wall->y1 == ySource)
        return vector;

    Segment seg = Segment_CreateFromCoords(wall->x1, wall->y1, wall->x2, wall->y2, xSource, ySource);
    (*vector++) = seg;

    double xInter = Segment_CheckXIntersection(seg, ySource);
    // Intersecção com a reta
    if (xInter <= xSource && xInter <= max(wall->x1, wall->x2) && xInter >= min(wall->x1, wall->x2)) {
        vector = Segment_Cut(seg, vector, xInter, xSource, ySource);
    }

    return vector;
}

double Wall_GetX1(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return wall->x1;
}

double Wall_GetY1(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return wall->y1;
}

double Wall_GetX2(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return wall->x2;
}

double Wall_GetY2(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return wall->y2;
}

void Wall_SetX1(Wall wallVoid, double x1) {
    WallPtr wall = (WallPtr) wallVoid;
    wall->x1 = x1;
}

void Wall_SetY1(Wall wallVoid, double y1) {
    WallPtr wall = (WallPtr) wallVoid;
    wall->y1 = y1;
}

void Wall_SetX2(Wall wallVoid, double x2) {
    WallPtr wall = (WallPtr) wallVoid;
    wall->x2 = x2;
}

void Wall_SetY2(Wall wallVoid, double y2) {
    WallPtr wall = (WallPtr) wallVoid;
    wall->y2 = y2;
}

void Wall_Destroy(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    free(wall);
}
