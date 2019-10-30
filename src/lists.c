#include "lists.h"

StList objList;
StList blockList;
StList hydList;
StList cTowerList;
StList tLightList;
StList buildingList;
StList wallList;

void initializeLists() {
    objList = StList_Create(DEFAULT_MAXIMUM);
    blockList = StList_Create(DEFAULT_MAXIMUM);
    hydList = StList_Create(DEFAULT_MAXIMUM);
    cTowerList = StList_Create(DEFAULT_MAXIMUM);
    tLightList = StList_Create(DEFAULT_MAXIMUM);
    buildingList = StList_Create(DEFAULT_MAXIMUM);
    wallList = StList_Create(DEFAULT_MAXIMUM);
}

void resizeLists(int nx, int nb, int nh, int nc, int nt, int np, int nw) {
    StList_Resize(objList, nx);
    StList_Resize(blockList, nb);
    StList_Resize(hydList, nh);
    StList_Resize(cTowerList, nc);
    StList_Resize(tLightList, nt);
    StList_Resize(buildingList, np);
    StList_Resize(wallList, nw);
}

void destroyLists() {
    StList_Destroy(objList, Object_Destroy);
    StList_Destroy(blockList, Block_Destroy);
    StList_Destroy(hydList, Equip_Destroy);
    StList_Destroy(cTowerList, Equip_Destroy);
    StList_Destroy(tLightList, Equip_Destroy);
    StList_Destroy(buildingList, Building_Destroy);
    StList_Destroy(wallList, Wall_Destroy);
}

StList getObjList() {
    return objList;
}

StList getBlockList() {
    return blockList;
}

StList getHydList() {
    return hydList;
}

StList getCTowerList() {
    return cTowerList;
}

StList getTLightList() {
    return tLightList;
}

StList getBuildingList() {
    return buildingList;
}

StList getWallList() {
    return wallList;
}
