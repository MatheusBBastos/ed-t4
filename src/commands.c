#include <string.h>
#include "commands.h"
#include "file_util.h"

bool processGeometry(FILE *entryFile);

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, char outputDir[], char svgFileName[]);

void writeObject(Object o, void *param) {
    FILE *svgFile = (FILE *) param;
    if (Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    } else if (Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    }
}

void writeText(Text text, void *param) {
    FILE *svgFile = (FILE *) param;
    putSVGText(svgFile, Text_GetX(text), Text_GetY(text), Text_GetString(text));
}

void writeSVG(FILE *outputSVGFile, bool svgTag) {
    if (svgTag)
        putSVGStart(outputSVGFile);
    RBTree_Execute(getObjTree(), writeObject, outputSVGFile);
    RBTree_Execute(getTextTree(), writeText, outputSVGFile);
    RBTree_Execute(getBlockTree(), putSVGBlock, outputSVGFile);
    RBTree_Execute(getBuildingTree(), putSVGBuilding, outputSVGFile);
    RBTree_Execute(getWallTree(), putSVGWall, outputSVGFile);
    RBTree_Execute(getHydTree(), putSVGHydrant, outputSVGFile);
    RBTree_Execute(getTLightTree(), putSVGTrafficLight, outputSVGFile);
    RBTree_Execute(getCTowerTree(), putSVGCellTower, outputSVGFile);
    if (svgTag)
        putSVGEnd(outputSVGFile);
}

//void processAll(FILE *entryFile, FILE *outputSVGFile, FILE *outputQryFile, FILE *queryFile, FILE *txtFile, char outputDir[], char svgFileName[]) {
void processAll(Files files) {
    initializeTrees();
    initializeTables();
    
    processGeometry(Files_GetEntryFile(files));
    writeSVG(Files_GetOutputSVGFile(files), true);
    
    if (Files_GetQueryFile(files) != NULL) {
        processAndGenerateQuery(files);
    }

    destroyTables();
    destroyTrees();
}

void processAndGenerateQuery(Files files) {
    FILE *outputQryFile = Files_GetOutputQryFile(files);
    putSVGStart(outputQryFile);
    putSVGQueryStart(outputQryFile);
    processQuery(Files_GetQueryFile(files), outputQryFile, Files_GetTxtFile(files), 
                    Files_GetOutputDir(files), Files_GetSvgFileName(files));
    putSVGQueryEnd(outputQryFile);
    writeSVG(outputQryFile, false);
    putSVGUseQuery(outputQryFile);
    putSVGEnd(outputQryFile);
}

bool processGeometry(FILE *entryFile) {
    int nx = DEFAULT_MAXIMUM;
    int nb = DEFAULT_MAXIMUM;
    int nh = DEFAULT_MAXIMUM;
    int nc = DEFAULT_MAXIMUM;
    int nt = DEFAULT_MAXIMUM;
    int np = DEFAULT_MAXIMUM;
    int nw = DEFAULT_MAXIMUM;

    char cFillBlock[24];
    char cStrkBlock[24];
    char wStrkBlock[16];

    char cFillHydrant[24];
    char cStrkHydrant[24];
    char wStrkHydrant[16];

    char cFillCellTower[24];
    char cStrkCellTower[24];
    char wStrkCellTower[16];

    char cFillTrafficLight[24];
    char cStrkTrafficLight[24];
    char wStrkTrafficLight[16];

    char wStrkCircle[16] = "2";
    char wStrkRectangle[16] = "2";

    char buffer[128];
    while (fgets(buffer, 100, entryFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);
        if (strcmp(type, "nx") == 0) {
            int nx1, nb1, nh1, nc1, nt1, np1, nw1;
            sscanf(buffer + 3, "%d %d %d %d %d %d %d", &nx1, &nb1, &nh1, &nc1, &nt1, &np1, &nw1);

            if (nx != nx1 || nb != nb1 || nh != nh1 || nc != nc1 || nt != nt1 || np != np1 || nw != nw1) {
                nx = nx1;
                nb = nb1;
                nh = nh1;
                nc = nc1;
                nt = nt1;
                np = np1;
                nw = nw1;
                //resizeLists(nx, nb, nh, nc, nt, np, nw);
            }
        } else if (strcmp(type, "c") == 0) {
            char id[8];
            double radius, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %s %s", id, &radius, &x, &y, color1, color2);

            Circle c = Circle_Create(radius, x, y);
            Object o = Object_Create(id, c, OBJ_CIRC, color1, color2, wStrkCircle);

            Object replacedObject = RBTree_Insert(getObjTree(), Circle_GetPoint(c), o);
            if (replacedObject != NULL)
                Object_Destroy(replacedObject);

        } else if (strcmp(type, "r") == 0) {
            char id[8];
            double width, height, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %lf %s %s", id, &width, &height, &x, &y, color1, color2);

            Rectangle r = Rectangle_Create(width, height, x, y);
            Object o = Object_Create(id, r, OBJ_RECT, color1, color2, wStrkRectangle);

            Object replacedObject = RBTree_Insert(getObjTree(), Rectangle_GetPoint(r), o);
            if (replacedObject != NULL)
                Object_Destroy(replacedObject);

        } else if (strcmp(type, "t") == 0) {
            double x, y;
            char string[128];
            sscanf(buffer + 2, "%lf %lf %128[^\n]", &x, &y, string);

            Text t = Text_Create(x, y, string);

            Text replacedText = RBTree_Insert(getTextTree(), Text_GetPoint(t), t);
            if (replacedText != NULL)
                Text_Destroy(replacedText);

        } else if (strcmp(type, "cq") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillBlock, cStrkBlock, wStrkBlock);
        } else if (strcmp(type, "ch") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillHydrant, cStrkHydrant, wStrkHydrant);
        } else if (strcmp(type, "cr") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillCellTower, cStrkCellTower, wStrkCellTower);
        } else if (strcmp(type, "cs") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
        } else if (strcmp(type, "q") == 0) {
            char cep[16];
            double x, y, w, h;
            sscanf(buffer + 2, "%15s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            Block block = Block_Create(cep, x, y, w, h, cFillBlock, cStrkBlock, wStrkBlock);

            Block replacedBlock = RBTree_Insert(getBlockTree(), Block_GetPoint(block), block);
            if (replacedBlock != NULL)
                Block_Destroy(replacedBlock);

            HashTable_Insert(getBlockTable(), Block_GetCep(block), block);

        } else if (strcmp(type, "h") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip hydrant = Equip_Create(id, x, y, cFillHydrant, cStrkHydrant, wStrkHydrant);

            Equip replacedEquip = RBTree_Insert(getHydTree(), Equip_GetPoint(hydrant), hydrant);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getHydTable(), Equip_GetID(hydrant), hydrant);

        } else if (strcmp(type, "s") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip trLight = Equip_Create(id, x, y, cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
            
            Equip replacedEquip = RBTree_Insert(getTLightTree(), Equip_GetPoint(trLight), trLight);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getTLightTable(), Equip_GetID(trLight), trLight);

        } else if (strcmp(type, "rb") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip cellTower = Equip_Create(id, x, y, cFillCellTower, cStrkCellTower, wStrkCellTower);
            
            Equip replacedEquip = RBTree_Insert(getCTowerTree(), Equip_GetPoint(cellTower), cellTower);
            if (replacedEquip != NULL)
                Equip_Destroy(replacedEquip);

            HashTable_Insert(getCTowerTable(), Equip_GetID(cellTower), cellTower);

        } else if (strcmp(type, "prd") == 0) {
            char cep[16];
            char face;
            double num, f, p, mrg;
            sscanf(buffer + 3, "%15s %c %lf %lf %lf %lf", cep, &face, &num, &f, &p, &mrg);

            Block b = HashTable_Find(getBlockTable(), cep);
            if (b == NULL) {
                #ifdef __DEBUG__
                printf("Erro: Quadra de CEP %s não encontrada!\n", cep);
                #endif
                continue;
            }

            double x, y, w, h;
            x = Block_GetX(b);
            y = Block_GetY(b);

            if (face == 'N' || face == 'n') {
                x += num - f / 2;
                y += Block_GetH(b);
                y -= mrg;
                y -= p;
                w = f;
                h = p;
            } else if (face == 'S' || face == 's') {
                x += num - f / 2;
                y += mrg;
                w = f;
                h = p;
            } else if (face == 'L' || face == 'l') {
                x += mrg;
                y += num - f / 2;
                w = p;
                h = f;
            } else if (face == 'O' || face == 'o') {
                x += Block_GetW(b);
                x -= mrg;
                x -= p;
                y += num - f / 2;
                w = p;
                h = f;
            }

            Building building = Building_Create(x, y, w, h, num);

            Building replacedBuilding = RBTree_Insert(getBuildingTree(), Building_GetPoint(building), building);
            if (replacedBuilding != NULL)
                Building_Destroy(replacedBuilding);

        } else if (strcmp(type, "mur") == 0) {
            double x1, y1, x2, y2;
            sscanf(buffer + 3, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2);

            Wall wall = Wall_Create(x1, y1, x2, y2);

            Wall replacedWall = RBTree_Insert(getWallTree(), Wall_GetPoint1(wall), wall);
            if (replacedWall != NULL)
                Wall_Destroy(wall);
        }
    }
    return true;
}

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, char outputDir[], char svgFileName[]) {
    char buffer[128];
    while (fgets(buffer, 128, queryFile) != NULL) {

        int len = strlen(buffer);
        if (buffer[len - 1] != '\n') {
            buffer[len] = '\n';
            buffer[len + 1] = '\0';
        }

        char type[16];
        sscanf(buffer, "%15s", type);
        if (strcmp(type, "o?") == 0) {

            char idA[8], idB[8];
            sscanf(buffer + 3, "%7s %7s", idA, idB);
            
            fputs(buffer, txtFile);
            if (!Query_Overlaps(txtFile, outputFile, idA, idB))
                return false;

        } else if (strcmp(type, "i?") == 0) {
            
            char id[8];
            double x, y;
            sscanf(buffer + 3, "%7s %lf %lf", id, &x, &y);

            fputs(buffer, txtFile);
            if (!Query_Inside(txtFile, outputFile, id, x, y))
                return false;

        } else if (strcmp(type, "d?") == 0) {

            char j[8], k[8];
            sscanf(buffer + 3, "%7s %7s", j, k);
            
            if (!Query_Distance(txtFile, outputFile, j, k))
                return false;

        } else if (strcmp(type, "bb") == 0) {

            char suffix[32], color[16];
            sscanf(buffer + 3, "%s %s", suffix, color);

            if (!Query_Bb(txtFile, outputFile, outputDir, svgFileName, suffix, color))
                return false;
        
        } else if (strcmp(type, "dq") == 0) {

            char metric[8], id[16];
            double dist;
            sscanf(buffer + 3, "%7s %15s %lf", metric, id, &dist);

            fputs(buffer, txtFile);
            if (!Query_Dq(txtFile, metric, id, dist))
                return false;
        
        } else if (strcmp(type, "del") == 0) {

            char id[16];
            sscanf(buffer + 4, "%15s", id);

            fputs(buffer, txtFile);
            if (!Query_Del(txtFile, id))
                return false;
        
        } else if (strcmp(type, "cbq") == 0) {

            double x, y, r;
            char cStrk[24];
            sscanf(buffer + 4, "%lf %lf %lf %23s", &x, &y, &r, cStrk);

            fputs(buffer, txtFile);
            if (!Query_Cbq(txtFile, x, y, r, cStrk))
                return false;
        
        } else if (strcmp(type, "crd?") == 0) {

            char id[16];
            sscanf(buffer + 5, "%15s", id);

            fputs(buffer, txtFile);
            if (!Query_Crd(txtFile, id))
                return false;

        } else if (strcmp(type, "trns") == 0) {

            double x, y, w, h, dx, dy;
            sscanf(buffer + 5, "%lf %lf %lf %lf %lf %lf", 
                   &x, &y, &w, &h, &dx, &dy);

            fputs(buffer, txtFile);
            if (!Query_Trns(txtFile, x, y, w, h, dx, dy))
                return false;

        } else if (strcmp(type, "brl") == 0) {

            double x, y;
            sscanf(buffer + 4, "%lf %lf", &x, &y);

            if (!Query_Brl(outputFile, x, y))
                return false;

        } else if (strcmp(type, "fi") == 0) {

            double x, y, r;
            int ns;

            sscanf(buffer + 3, "%lf %lf %d %lf", &x, &y, &ns, &r);

            fputs(buffer, txtFile);
            if (!Query_Fi(txtFile, outputFile, x, y, ns, r))
                return false;

        } else if (strcmp(type, "fh") == 0) {

            char signal, cep[16], face;
            double num;
            int k;

            sscanf(buffer + 3, "%c%d %15s %c %lf", &signal, &k, cep, &face, &num);

            fputs(buffer, txtFile);
            if (!Query_Fh(txtFile, outputFile, signal, k, cep, face, num))
                return false;

        } else if (strcmp(type, "fs") == 0) {

            char cep[16], face;
            double num;
            int k;

            sscanf(buffer + 3, "%d %15s %c %lf", &k, cep, &face, &num);

            fputs(buffer, txtFile);
            if (!Query_Fs(txtFile, outputFile, k, cep, face, num))
                return false;

        }
    }
    return true;
}