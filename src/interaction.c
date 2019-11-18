#include "interaction.h"

#ifndef CURSES_ON
void startInteraction(Files *files, char *baseDir, char *entryFileName) {
    printf("-- MODO INTERAÇÃO --\n");
    char buffer[128];
    char command[16];
 
    do {
        fgets(buffer, 100, stdin);
        sscanf(buffer, "%15s", command);

        if (strcmp(command, "q") == 0) {
            char qryFileName[64];
            sscanf(buffer + 2, "%63[^\n]", qryFileName);

            if (!Files_OpenQueryFiles(files, baseDir, entryFileName, qryFileName))
                continue;
            
            processAndGenerateQuery(files);
        } else if (strcmp(command, "dmprbt") == 0) {
            char t = '\0', arq[64] = "";
            sscanf(buffer + 7, "%c %63[^\n]", &t, arq);

            if (t == '\0') {
                printf("Forneça um tipo!\n");
                continue;
            }

            if (arq[0] == '\0') {
                printf("Forneça um arquivo!\n");
                continue;
            }

            if (Query_Dmprbt(Files_GetOutputDir(files), t, arq))
                printf("Árvore escrita no arquivo %s!\n", arq);

        } else if (strcmp(command, "nav") == 0) {
            char t;
            sscanf(buffer + 4, "%c", &t);

            RBTree tree = NULL;

            switch (t) {
                case 'q':
                    tree = getBlockTree();
                    break;
                case 'h':
                    tree = getHydTree();
                    break;
                case 's':
                    tree = getTLightTree();
                    break;
                case 't':
                    tree = getCTowerTree();
                    break;
                case 'p':
                    tree = getBuildingTree();
                    break;
                case 'm':
                    tree = getWallTree();
                    break;
            }

            if (tree == NULL) {
                printf("Árvore inexistente: %c!\n", t);
                continue;
            }

            Node node = RBTree_GetRoot(tree);
            Node parentNode = NULL;

            char internalCommand;
            do {
                printf("Nó atual\n");
                if (node == NULL) {
                    printf("- Cor: PRETA\n- nil\n");
                } else {
                    printf("- Cor: %s\n", RBTreeN_GetColor(node) == RED ? "vermelho" : "preto");
                    Wall wall;
                    switch (t) {
                        case 'q':
                            printf("- Quadra %s\n", Block_GetCep(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'h':
                            printf("- Hidrante %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 's':
                            printf("- Semáforo %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 't':
                            printf("- Rádio-base %s\n", Equip_GetID(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'p':
                            printf("- Prédio %s\n", Building_GetKey(RBTreeN_GetValue(tree, node)));
                            break;
                        case 'm':
                            wall = RBTreeN_GetValue(tree, node);
                            printf("- Muro (%.2lf, %.2lf) - (%.2lf, %.2lf)\n",
                                    Wall_GetX1(wall), Wall_GetY1(wall),
                                    Wall_GetX2(wall), Wall_GetY2(wall));
                            break;
                    }
                }

                scanf("%c%*c", &internalCommand);
                if (internalCommand == 'e') {
                    if (node == NULL) {
                        printf("Nó folha!\nAperte ENTER\n");
                        scanf("%*c");
                        continue;
                    }
                    parentNode = node;
                    node = RBTreeN_GetLeftChild(tree, node);
                } else if (internalCommand == 'd') {
                    if (node == NULL) {
                        printf("Nó folha!\nAperte ENTER\n");
                        scanf("%*c");
                        continue;
                    }
                    parentNode = node;
                    node = RBTreeN_GetRightChild(tree, node);
                } else if (internalCommand == 'p') {
                    if (node == NULL)
                        node = parentNode;
                    else {
                        Node parent = RBTreeN_GetParent(tree, node);
                        if (parent == NULL) {
                            printf("Nó raiz!\nAperte ENTER\n");
                            scanf("%*c");
                        } else {
                            node = parent;
                        }
                    }
                }
            } while (internalCommand != 'x');
        }
    } while (strcmp(command, "sai") != 0);
}

#else
void startInteraction(Files *files, char *baseDir, char *entryFileName) {
    startGui(files, baseDir, entryFileName);
}

#endif
