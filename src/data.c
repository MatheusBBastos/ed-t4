#include "data.h"

RBTree objTree;
RBTree textTree;
RBTree blockTree;
RBTree hydTree;
RBTree cTowerTree;
RBTree tLightTree;
RBTree buildingTree;
RBTree wallTree;

HashTable blockTable;
HashTable objTable;
HashTable hydTable;
HashTable cTowerTable;
HashTable tLightTable;

void initializeTrees() {
    objTree = RBTree_Create(Point_Compare);
    textTree = RBTree_Create(Point_Compare);
    blockTree = RBTree_Create(Point_Compare);
    hydTree = RBTree_Create(Point_Compare);
    cTowerTree = RBTree_Create(Point_Compare);
    tLightTree = RBTree_Create(Point_Compare);
    buildingTree = RBTree_Create(Point_Compare);
    wallTree = RBTree_Create(Point_Compare);
}

void initializeTables() {
    blockTable = HashTable_Create(1001);
    objTable = HashTable_Create(1001);
    hydTable = HashTable_Create(1001);
    cTowerTable = HashTable_Create(1001);
    tLightTable = HashTable_Create(1001);
}

void destroyTrees() {
    RBTree_Destroy(objTree, Object_Destroy);
    RBTree_Destroy(textTree, Text_Destroy);
    RBTree_Destroy(blockTree, Block_Destroy);
    RBTree_Destroy(hydTree, Equip_Destroy);
    RBTree_Destroy(cTowerTree, Equip_Destroy);
    RBTree_Destroy(tLightTree, Equip_Destroy);
    RBTree_Destroy(buildingTree, Building_Destroy);
    RBTree_Destroy(wallTree, Wall_Destroy);
}

void destroyTables() {
    HashTable_Destroy(blockTable, NULL);
    HashTable_Destroy(objTable, NULL);
    HashTable_Destroy(hydTable, NULL);
    HashTable_Destroy(cTowerTable, NULL);
    HashTable_Destroy(tLightTable, NULL);
}

RBTree getObjTree() {
   return objTree;
}

HashTable getObjTable() {
    return objTable;
}

RBTree getTextTree() {
    return textTree;
}

RBTree getBlockTree() {
    return blockTree;
}

HashTable getBlockTable() {
    return blockTable;
}

RBTree getHydTree() {
    return hydTree;
}

RBTree getCTowerTree() {
    return cTowerTree;
}

RBTree getTLightTree() {
    return tLightTree;
}

RBTree getBuildingTree() {
    return buildingTree;
}

RBTree getWallTree() {
    return wallTree;
}

HashTable getHydTable() {
    return hydTable;
}

HashTable getCTowerTable() {
    return cTowerTable;
}

HashTable getTLightTable() {
    return tLightTable;
}
