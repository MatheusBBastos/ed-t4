#include "building.h"

typedef struct building_t {
    double x;
    double y;
    double w;
    double h;
    double num;
} *BuildingPtr;

Building Building_Create(double x, double y, double w, double h, double num) {
    BuildingPtr building = malloc(sizeof(struct building_t));
    building->x = x;
    building->y = y;
    building->w = w;
    building->h = h;
    building->num = num;
    return building;
}

Segment *Building_PutSegments(Building buildingVoid, Segment *vector, double xSource, double ySource) {
    BuildingPtr b = (BuildingPtr) buildingVoid;

    double xMin = b->x;
    double xMax = b->x + b->w;
    double yMin = b->y;
    double yMax = b->y + b->h;
    
    Segment southSeg;
    if (yMin != ySource) {
        southSeg = Segment_CreateFromCoords(xMin, yMin, xMax, yMin, xSource, ySource);
        *(vector++) = southSeg;
    }
    
    Segment westSeg;
    if (xMax != xSource) {
        westSeg = Segment_CreateFromCoords(xMax, yMin, xMax, yMax, xSource, ySource);
        *(vector++) = westSeg;
    }

    Segment northSeg;
    if (yMax != ySource) {
        northSeg = Segment_CreateFromCoords(xMin, yMax, xMax, yMax, xSource, ySource);
        *(vector++) = northSeg;
    }

    Segment eastSeg;
    if (xMin != xSource) {
        eastSeg = Segment_CreateFromCoords(xMin, yMin, xMin, yMax, xSource, ySource);
        *(vector++) = eastSeg;
    }

    // Intersecção com a reta
    if (b->x < xSource && ySource > b->y && ySource < b->y + b->h) {
        // Segmento leste
        vector = Segment_Cut(eastSeg, vector, b->x, xSource, ySource);
        // Segmento oeste
        vector = Segment_Cut(westSeg, vector, b->x + b->w, xSource, ySource);
    }

    return vector;
}

double Building_GetX(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->x;
}

double Building_GetY(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->y;
}

double Building_GetW(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->w;
}

double Building_GetH(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->h;
}

double Building_GetNum(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->num;
}

void Building_SetX(Building buildingVoid, double x) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->x = x;
}

void Building_SetY(Building buildingVoid, double y) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->y = y;
}

void Building_SetW(Building buildingVoid, double w) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->w = w;
}

void Building_SetH(Building buildingVoid, double h) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->h = h;
}

void Building_SetNum(Building buildingVoid, double num) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->num = num;
}

void Building_Destroy(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    free(building);
}
