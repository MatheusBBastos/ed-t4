#include "query.h"

typedef struct BBParameters {
    FILE *file;
    char *color;
} BBParameters;

static void insertBoundingBoxElement(void *o, void *parametersVoid) {
    BBParameters *params = (BBParameters*) parametersVoid;
    FILE *file = params->file;
    if (Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        
        Rectangle surroundingRect = Rectangle_Create(0, 0, 0, 0);
        getSurroundingRect(o, surroundingRect);
        
        putSVGRectangle(file, surroundingRect, params->color, "none", "2");
        Rectangle_Destroy(surroundingRect);
    } else if (Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        Rectangle rect = Object_GetContent(o);
        double cx, cy;
        getCenter(o, &cx, &cy);
        double rx = Rectangle_GetWidth(rect)/2, ry = Rectangle_GetHeight(rect)/2;
        putSVGEllipse(file, cx, cy, rx, ry, params->color, "none");
    }
}

static bool blockInDistanceL1(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

static bool blockInDistanceL2(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

bool Query_Overlaps(FILE *txtFile, FILE *outputFile, char idA[], char idB[]) {
    Object a = HashTable_Find(getObjTable(), idA);
    Object b = HashTable_Find(getObjTable(), idB);
    if (a == NULL || b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }
    
    bool overlaps = checkOverlap(a, b);
    if (overlaps) {
        fprintf(txtFile, "SIM\n\n");
    } else {
        fprintf(txtFile, "NAO\n\n");
    }
    Rectangle rectA = Rectangle_Create(0, 0, 0, 0);
    getSurroundingRect(a, rectA);
    Rectangle rectB = Rectangle_Create(0, 0, 0, 0);
    getSurroundingRect(b, rectB);
    double minX = Rectangle_GetX(rectA) < Rectangle_GetX(rectB) ? Rectangle_GetX(rectA) : Rectangle_GetX(rectB);
    double minY = Rectangle_GetY(rectA) < Rectangle_GetY(rectB) ? Rectangle_GetY(rectA) : Rectangle_GetY(rectB);
    double maxX = Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA) < Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) 
        ? Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) : Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA);
    double maxY = Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA) < Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) 
        ? Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) : Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA);
    putSVGBox(outputFile, minX, minY, maxX - minX, maxY - minY, !overlaps);
    Rectangle_Destroy(rectA);
    Rectangle_Destroy(rectB);

    return true;
}

bool Query_Inside(FILE *txtFile, FILE *outputFile, char id[], double x, double y) {
    Object o = HashTable_Find(getObjTable(), id);
    if (o == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    bool inside = checkInside(o, x, y);
    if (inside) {
        fprintf(txtFile, "INTERNO\n\n");
    } else {
        fprintf(txtFile, "NAO INTERNO\n\n");
    }
    double centerX, centerY;
    getCenter(o, &centerX, &centerY);
    putSVGLine(outputFile, centerX, centerY, x, y);
    putSVGPoint(outputFile, x, y, inside);

    return true;
}

bool Query_Distance(FILE *txtFile, FILE *outputFile, char j[], char k[]) {
    double c1x, c1y, c2x, c2y;
    Object a = HashTable_Find(getObjTable(), j);
    Object b = HashTable_Find(getObjTable(), k);
    if (a == NULL || b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }
    getCenter(a, &c1x, &c1y);
    getCenter(b, &c2x, &c2y);
    double dist = euclideanDistance(c1x, c1y, c2x, c2y);
    fprintf(txtFile, "%lf\n\n", dist);
    putSVGLine(outputFile, c1x, c1y, c2x, c2y);
    char distText[16];
    sprintf(distText, "%lf", dist);
    putSVGText(outputFile, c1x + (c2x - c1x) / 2, c1y + (c2y - c1y) / 2, distText);
}

bool Query_Bb(FILE *txtFile, FILE *outputFile, char outputDir[], char svgFileName[], char suffix[], char color[]) {
    char nameWithSuffix[128];
    strcpy(nameWithSuffix, svgFileName);
    addSuffix(nameWithSuffix, suffix);
    FILE *bbFile = openFile(outputDir, nameWithSuffix, "w");
    if (bbFile == NULL) {
        return true;
    }

    putSVGStart(bbFile);
    BBParameters params = {bbFile, color};
    RBTree_Execute(getObjTree(), insertBoundingBoxElement, &params);
    putSVGEnd(bbFile);
    fclose(bbFile);
}

bool Query_Dq(FILE *txtFile, char metric[], char id[], double dist) {
    Equip e = HashTable_Find(getHydTable(), id);
    if (e == NULL)
        e = HashTable_Find(getCTowerTable(), id);
    if (e == NULL)
        e = HashTable_Find(getTLightTable(), id);
    if (e == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
        return true;
    }

    fprintf(txtFile, "Equipamento ID: %s\n", Equip_GetID(e));

    bool (*_blockInDistance)(void *, void *);
    if (strcmp(metric, "L1") == 0) {
        _blockInDistance = blockInDistanceL1;
    } else if (strcmp(metric, "L2") == 0) {
        _blockInDistance = blockInDistanceL2;
    } else {
        printf("Métrica não reconhecida: %s\n", metric);
        fprintf(txtFile, "Métrica não reconhecida: %s\n\n", metric);
        return true;
    }

    Equip_SetHighlighted(e, true);
    fprintf(txtFile, "Quadras removidas: ");

    double dInfos[3] = {Equip_GetX(e), Equip_GetY(e), dist};
    Block b;
    while (b = RBTree_FindWhere(getBlockTree(), _blockInDistance, (void *) dInfos), b != NULL) {
        fprintf(txtFile, "\n\t- %s", Block_GetCep(b));
        RBTree_Remove(getBlockTree(), Block_GetPoint(b));
        HashTable_Remove(getBlockTable(), Block_GetCep(b));
        Block_Destroy(b);
    }

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Del(FILE *txtFile, char id[]) {
    Block b = HashTable_Find(getBlockTable(), id);
    if (b != NULL) {
        fprintf(txtFile, "Informações da quadra removida:\n"
                            "CEP: %s\nPos: (%.2lf, %.2lf)\n"
                            "Largura: %.2lf\nAltura: %.2lf\n\n",
                            Block_GetCep(b), Block_GetX(b), Block_GetY(b), 
                            Block_GetW(b), Block_GetH(b));
        RBTree_Remove(getBlockTree(), Block_GetPoint(b));
        HashTable_Remove(getBlockTable(), id);
        Block_Destroy(b);
        return true;
    }

    Equip e = HashTable_Find(getHydTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações do hidrante removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        RBTree_Remove(getHydTree(), Equip_GetPoint(e));
        HashTable_Remove(getHydTable(), id);
        Equip_Destroy(e);

        return true;
    }

    e = HashTable_Find(getCTowerTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações da rádio-base removida:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        
        RBTree_Remove(getCTowerTree(), Equip_GetPoint(e));
        HashTable_Remove(getCTowerTable(), id);
        Equip_Destroy(e);

        return true;
    }

    e = HashTable_Find(getTLightTable(), id);
    if (e != NULL) {
        fprintf(txtFile, "Informações do semáforo removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        
        RBTree_Remove(getTLightTree(), Equip_GetPoint(e));
        HashTable_Remove(getTLightTable(), id);
        Equip_Destroy(e);

        return true;
    }
    

    fprintf(txtFile, "Elemento não encontrado\n\n");
    #ifdef __DEBUG__
    printf("Erro: Elemento não encontrado: %s!\n", id);
    #endif
    return true;
}


typedef struct InfosCbq {
    double dInfos[3];
    char *cStrk;
    FILE *txtFile;
} InfosCbq;

static bool _canBeOnLeftSubtree(double x, double r, double xChild) {
    return xChild >= x - r;
}

static bool _canBeOnRightSubtree(double x, double r, double xChild) {
    return xChild <= x + r;
}

static void _changeBlockColorTree(RBTree tree, Node node, InfosCbq *infos) {
    if (node == NULL)
        return;
    double *d = infos->dInfos;
    Block b = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(d[0], d[2], Block_GetX(b)))
        _changeBlockColorTree(tree, RBTreeN_GetLeftChild(tree, node), infos);
    if (blockInDistanceL2(b, (void *) d)) {
        fprintf(infos->txtFile, "\n\t- %s", Block_GetCep(b));
        Block_SetCStroke(b, infos->cStrk);
    }
    if (_canBeOnRightSubtree(d[0], d[2], Block_GetX(b))) {
        _changeBlockColorTree(tree, RBTreeN_GetRightChild(tree, node), infos);
    }
}

bool Query_Cbq(FILE *txtFile, double x, double y, double r, char cStrk[]) {
    fprintf(txtFile, "Quadras que tiveram as bordas alteradas: ");

    InfosCbq infos = {{x, y, r}, cStrk, txtFile};
    _changeBlockColorTree(getBlockTree(), RBTree_GetRoot(getBlockTree()), &infos);

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Crd(FILE *txtFile, char id[]) {
    char eqType[24] = "";
    double x, y;

    Equip e;
    Block b;
    if (b = HashTable_Find(getBlockTable(), id), b != NULL) {
        strcpy(eqType, "Quadra");
        x = Block_GetX(b);
        y = Block_GetY(b);
    } else if (e = HashTable_Find(getHydTable(), id), e != NULL) {
        strcpy(eqType, "Hidrante");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if (e = HashTable_Find(getCTowerTable(), id), e != NULL) {
        strcpy(eqType, "Rádio-base");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if (e = HashTable_Find(getTLightTable(), id), e != NULL) {
        strcpy(eqType, "Semáforo");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    }

    if (eqType[0] != '\0') {
        fprintf(txtFile, "Espécie do equipamento: %s\n"
                            "Pos: (%.2lf, %.2lf)\n\n",
                            eqType, x, y);
    } else {
        fprintf(txtFile, "Elemento não encontrado\n\n");
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado: %s!\n", id);
        #endif
    }
    return true;
}

//
// Comando trns
//

typedef struct InfosTrns {
    double x, y, w, h;
} InfosTrns;

typedef struct list_node_t {
    void *element;
    RBTree tree;
    struct list_node_t *next;
} ListNode;

static void _translateBlockTree(RBTree tree, Node node, InfosTrns *infos, ListNode **list) {
    if (node == NULL)
        return;
    Block block = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(infos->x + infos->w, infos->w, Block_GetX(block)))
        _translateBlockTree(tree, RBTreeN_GetLeftChild(tree, node), infos, list);

    if (Block_GetX(block) >= infos->x && Block_GetX(block) + Block_GetW(block) <= infos->x + infos->w &&
            Block_GetY(block) >= infos->y && Block_GetY(block) + Block_GetH(block) <= infos->y + infos->h) {
        // Se a quadra estiver dentro do retângulo, adicioná-la à lista
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = block;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }

    if (_canBeOnRightSubtree(infos->x, infos->x + infos->w, Block_GetX(block))) {
        _translateBlockTree(tree, RBTreeN_GetRightChild(tree, node), infos, list);
    }
}

static void _translateEquipTree(RBTree tree, Node node, InfosTrns *infos, ListNode **list) {
    if (node == NULL)
        return;
    Equip equip = RBTreeN_GetValue(tree, node);
    if (_canBeOnLeftSubtree(infos->x + infos->w, infos->w, Equip_GetX(equip)))
        _translateEquipTree(tree, RBTreeN_GetLeftChild(tree, node), infos, list);

    if (Equip_GetX(equip) >= infos->x && Equip_GetX(equip) <= infos->x + infos->w &&
            Equip_GetY(equip) >= infos->y && Equip_GetY(equip) <= infos->y + infos->h) {
        // Se o equipamento estiver dentro do retângulo, adicioná-lo à lista
        ListNode *node = malloc(sizeof(struct list_node_t));
        node->element = equip;
        node->tree = tree;
        node->next = NULL;
        (*list)->next = node;
        *list = node;
    }

    if (_canBeOnRightSubtree(infos->x, infos->x + infos->w, Equip_GetX(equip))) {
        _translateEquipTree(tree, RBTreeN_GetRightChild(tree, node), infos, list);
    }
}

bool Query_Trns(FILE *txtFile, double x, double y, double w, double h, double dx, double dy) {
    InfosTrns infos;
    
    infos.x = x;
    infos.y = y;
    infos.w = w;
    infos.h = h;

    fprintf(txtFile, "Equipamentos movidos:");
    
    // Primeiro nó vazio
    ListNode *node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    ListNode *nodeP = node;

    // Preencher lista
    _translateBlockTree(getBlockTree(), RBTree_GetRoot(getBlockTree()), &infos, &nodeP);
    
    // Descartar primeiro nó (vazio)
    ListNode *next = node->next;
    free(node);
    node = next;

    // Percorrer a lista de quadras a serem transladadas
    while (node != NULL) {
        // Remover devido à mudança de chave
        Block block = RBTree_Remove(node->tree, Block_GetPoint(node->element));

        fprintf(txtFile, "\n%s:"
                         "\n\tPosição anterior: (%.2lf, %.2lf)"
                         "\n\tNova posição: (%.2lf, %.2lf)",
                         Block_GetCep(block), Block_GetX(block), Block_GetY(block),
                         Block_GetX(block) + dx, Block_GetY(block) + dy);

        Block_SetX(block, Block_GetX(block) + dx);
        Block_SetY(block, Block_GetY(block) + dy);

        // Inserir novamente com a chave nova
        RBTree_Insert(node->tree, Block_GetPoint(block), block);

        next = node->next;
        free(node);
        node = next;
    }

    // Primeiro nó vazio
    node = malloc(sizeof(struct list_node_t));
    node->next = NULL;
    nodeP = node;

    // Preencher lista
    _translateEquipTree(getHydTree(), RBTree_GetRoot(getHydTree()), &infos, &nodeP);
    _translateEquipTree(getCTowerTree(), RBTree_GetRoot(getCTowerTree()), &infos, &nodeP);
    _translateEquipTree(getTLightTree(), RBTree_GetRoot(getTLightTree()), &infos, &nodeP);

    // Descartar primeiro nó (vazio)
    next = node->next;
    free(node);
    node = next;

    // Percorrer a lista de equipamentos a serem transladados
    while (node != NULL) {
        // Remover devido à mudança de chave
        Equip equip = RBTree_Remove(node->tree, Equip_GetPoint(node->element));

        fprintf(txtFile, "\n%s:"
                         "\n\tPosição anterior: (%.2lf, %.2lf)"
                         "\n\tNova posição: (%.2lf, %.2lf)",
                         Equip_GetID(equip), Equip_GetX(equip), Equip_GetY(equip),
                         Equip_GetX(equip) + dx, Equip_GetY(equip) + dy);
        Equip_SetX(equip, Equip_GetX(equip) + dx);
        Equip_SetY(equip, Equip_GetY(equip) + dy);
        
        // Inserir novamente com a chave nova
        RBTree_Insert(node->tree, Equip_GetPoint(equip), equip);

        next = node->next;
        free(node);
        node = next;
    }
    
    fprintf(txtFile, "\n\n");

    return true;
}

static int compareDistancesDescending(const void *a, const void *b) {
    double d1 = Distance_GetDist(a);
    double d2 = Distance_GetDist(b);

    return d1 < d2 ? 1 
         : d1 > d2 ? -1 
         : 0;
}

static int compareDistancesAscending(const void *a, const void *b) {
    double d1 = Distance_GetDist(a);
    double d2 = Distance_GetDist(b);

    return d1 > d2 ? 1 
         : d1 < d2 ? -1 
         : 0;
}

static void buildDistances(RBTree tree, Node node, Distance **distances, int x, int y) {
    if (node == NULL)
        return;
    
    buildDistances(tree, RBTreeN_GetLeftChild(tree, node), distances, x, y);
    Equip equip = RBTreeN_GetValue(tree, node);
    double dist = euclideanDistance(Equip_GetX(equip), Equip_GetY(equip), x, y);
    // Adicionar distâcia ao vetor
    *((*distances)++) = Distance_Create(dist, equip);
    buildDistances(tree, RBTreeN_GetRightChild(tree, node), distances, x, y);
}

bool Query_Fi(FILE *txtFile, FILE *outputFile, double x, double y, int ns, double r) {
    fprintf(txtFile, "Semáforos com a programação alterada:");

    int n = RBTree_GetLength(getTLightTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos semáforos ao ponto (x, y)
    buildDistances(getTLightTree(), RBTree_GetRoot(getTLightTree()), &distancesP, x, y);

    // Ordenar as distâncias até que se tenha as 'ns' maiores
    heapsort(distances, n, ns, compareDistancesDescending);

    int limit = n - ns;
    for (int i = n - 1; i >= limit; i--) {
        Equip tLight = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(tLight, true);
        putSVGLine(outputFile, x, y, Equip_GetX(tLight), Equip_GetY(tLight));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(tLight));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n");
    fprintf(txtFile, "Hidrantes ativados:");

    // Percorrer a árvore de hidrantes
    for (Node node = RBTree_GetFirstNode(getHydTree()); node != NULL; node = RBTreeN_GetSuccessor(getHydTree(), node)) {
        Equip hydrant = RBTreeN_GetValue(getHydTree(), node);
        double dist = euclideanDistance(Equip_GetX(hydrant), Equip_GetY(hydrant), x, y);
        if (dist <= r) {
            Equip_SetHighlighted(hydrant, true);
            putSVGLine(outputFile, x, y, Equip_GetX(hydrant), Equip_GetY(hydrant));
            fprintf(txtFile, "\n\t- %s", Equip_GetID(hydrant));
        }
    }

    fprintf(txtFile, "\n\n");

    return true;
}

bool Query_Fh(FILE *txtFile, FILE *outputFile, char signal, int k, char cep[], char face, double num) {

    Block b = HashTable_Find(getBlockTable(), cep);
    if (b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    double x, y;

    if (!Block_GetCoordinates(b, face, num, &x, &y)) {
        #ifdef __DEBUG__
        printf("Erro: Direção desconhecida: '%c'!\n", face);
        #endif
        fprintf(txtFile, "Direção desconhecida: '%c'\n\n", face);
        return true;
    }

    int n = RBTree_GetLength(getHydTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos hidrantes ao ponto (x, y)
    buildDistances(getHydTree(), RBTree_GetRoot(getHydTree()), &distancesP, x, y);

    if (signal == '+') {
        fprintf(txtFile, "%d hidrantes mais distantes:", k);
        //qsort(distances, n, sizeof(EquipDist), compareDistancesAscending);
        heapsort(distances, n, k, compareDistancesAscending);
    } else if (signal == '-') {
        fprintf(txtFile, "%d hidrantes mais próximos:", k);
        //qsort(distances, n, sizeof(EquipDist), compareDistancesDescending);
        heapsort(distances, n, k, compareDistancesDescending);
    } else {
        #ifdef __DEBUG__
        printf("Erro: Sinal desconhecido: '%c'!\n", signal);
        #endif
        fprintf(txtFile, "Sinal desconhecido: '%c'\n\n", signal);
        free(distances);
        return true;
    }

    int limit = n - k;
    for (int i = n - 1; i >= limit; i--) {
        Equip hyd = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(hyd, true);
        putSVGLine(outputFile, x, y, Equip_GetX(hyd), Equip_GetY(hyd));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(hyd));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n\n");

    return true;
}

bool Query_Fs(FILE *txtFile, FILE *outputFile, int k, char cep[], char face, double num) {

    Block b = HashTable_Find(getBlockTable(), cep);
    if (b == NULL) {
        #ifdef __DEBUG__
        printf("Erro: Elemento não encontrado!\n");
        #endif
        fprintf(txtFile, "Elemento não encontrado\n\n");
        return true;
    }

    double x, y;

    if (!Block_GetCoordinates(b, face, num, &x, &y)) {
        #ifdef __DEBUG__
        printf("Erro: Direção desconhecida: '%c'!\n", face);
        #endif
        fprintf(txtFile, "Direção desconhecida: '%c'\n\n", face);
        return true;
    }

    int n = RBTree_GetLength(getTLightTree());
    Distance *distances = malloc(n * sizeof(Distance));
    Distance *distancesP = distances;

    // Construir vetor de distâncias dos semáforos ao ponto (x, y)
    buildDistances(getTLightTree(), RBTree_GetRoot(getTLightTree()), &distancesP, x, y);

    fprintf(txtFile, "%d semáforos mais próximos:", k);
    heapsort(distances, n, k, compareDistancesDescending);

    int limit = n - k;
    for (int i = n - 1; i >= limit; i--) {
        Equip hyd = Distance_GetEquip(distances[i]);
        Equip_SetHighlighted(hyd, true);
        putSVGLine(outputFile, x, y, Equip_GetX(hyd), Equip_GetY(hyd));
        fprintf(txtFile, "\n\t- %s", Equip_GetID(hyd));
    }

    for (int i = 0; i < n; i++)
        Distance_Destroy(distances[i]);
    free(distances);

    fprintf(txtFile, "\n\n");

    return true;
}

// bool compareAddr(Segment s1, Segment s2) {
//     return s1 == s2;
// }

bool Query_Brl(FILE *outputFile, double x, double y) {

    // int nBuildings = StList_GetNumElements(getBuildingList());
    // int nWalls = StList_GetNumElements(getWallList());
    int nBuildings = RBTree_GetLength(getBuildingTree());
    int nWalls = RBTree_GetLength(getWallTree());

    int nSegments = nBuildings * 4 + nWalls;

    Segment *segments = malloc((nSegments * 2 + 5) * sizeof(Segment));
    Segment *segmentsP = segments;

    double maxX = x, maxY = y;

    // Colocar segmentos dos prédios na lista
    //for (int p = StList_GetFirstPos(getBuildingList()); p != -1; p = StList_GetNextPos(getBuildingList(), p)) {
    for (Node node = RBTree_GetFirstNode(getBuildingTree()); node != NULL; node = RBTreeN_GetSuccessor(getBuildingTree(), node)) {
        //Building b = StList_Get(getBuildingList(), p);
        Building b = RBTreeN_GetValue(getBuildingTree(), node);
        segmentsP = Building_PutSegments(b, segmentsP, x, y);
        double blockMaxX = Building_GetX(b) + Building_GetW(b);
        if (blockMaxX > maxX)
            maxX = blockMaxX;
        double blockMaxY = Building_GetY(b) + Building_GetH(b);
        if (blockMaxY > maxY)
            maxY = blockMaxY;
    }

    // Colocar segmentos dos muros na lista
    //for (int p = StList_GetFirstPos(getWallList()); p != -1; p = StList_GetNextPos(getWallList(), p)) {
    for (Node node = RBTree_GetFirstNode(getWallTree()); node != NULL; node = RBTreeN_GetSuccessor(getWallTree(), node)) {
        //Wall w = StList_Get(getWallList(), p);
        Wall w = RBTreeN_GetValue(getWallTree(), node);
        segmentsP = Wall_PutSegments(w, segmentsP, x, y);
        double wallMaxX = max(Wall_GetX1(w), Wall_GetX2(w));
        if (wallMaxX > maxX)
            maxX = wallMaxX;
        double wallMaxY = max(Wall_GetY1(w), Wall_GetY2(w));
        if (wallMaxY > maxY)
            maxY = wallMaxY;
    }

    maxX += 100;
    maxY += 100;

    // Bordas da cidade
    Wall borders[4];
    borders[0] = Wall_Create(0, 0, maxX, 0);
    borders[1] = Wall_Create(maxX, 0, maxX, maxY);
    borders[2] = Wall_Create(maxX, maxY, 0, maxY);
    borders[3] = Wall_Create(0, maxY, 0, 0);
    for (int i = 0; i < 4; i++) {
        segmentsP = Wall_PutSegments(borders[i], segmentsP, x, y);
        Wall_Destroy(borders[i]);
    }

    // Novo número de segmentos
    nSegments = segmentsP - segments;

    int nVertexes = nSegments * 2;
    Vertex *vertexes = malloc(nVertexes * sizeof(Vertex));

    // Criar o vetor de pontos
    for (int i = 0; i < nSegments; i++) {
        Segment s = segments[i];
        vertexes[2 * i] = Segment_GetPStart(s);
        vertexes[2 * i + 1] = Segment_GetPEnd(s);
    }

    // Ordenar pontos
    qsort(vertexes, nVertexes, sizeof(Vertex), Vertex_Compare);

    //StList activeSegments = StList_Create(nSegments);
    RBTree activeSegments = RBTree_Create(Vertex_Compare);

    for (int i = 0; i < nVertexes; i++) {
        Vertex p = vertexes[i];
        Segment s = Vertex_GetSegment(p);
        double dist = Vertex_GetDistance(p);

        // Coeficiente angular da reta formada pelo ponto central e o ponto atual
        double a1;
        bool vertical = false;
        if (Vertex_GetX(p) == x)
            vertical = true;
        else
            a1 = (Vertex_GetY(p) - y) / (Vertex_GetX(p) - x);
        // Termo independente da reta
        double b1 = y - a1 * x;

        // Distância do segmento ativo mais perto que está atrás do segmento analisado
        double minDist = -1;
        Segment closestSegmentBehind = NULL;
        double xInter, yInter;
        bool inFront = true;

        //for (int pos = StList_GetFirstPos(activeSegments); pos != -1; pos = StList_GetNextPos(activeSegments, pos)) {
        for (Node node = RBTree_GetFirstNode(activeSegments); node != NULL; node = RBTreeN_GetSuccessor(activeSegments, node)) {
            //Segment currentSegment = StList_Get(activeSegments, pos);
            Segment currentSegment = RBTreeN_GetValue(activeSegments, node);
            if (currentSegment == s)
                continue;

            Vertex p1 = Segment_GetPStart(currentSegment);
            Vertex p2 = Segment_GetPEnd(currentSegment);

            // Ponto de intersecção entre a reta anterior e uma nova reta (formada pelo segmento analisado)
            double currentXInter, currentYInter;

            // Evitar divisão por zero
            if (Vertex_GetX(p2) == Vertex_GetX(p1)) {
                currentXInter = Vertex_GetX(p1);
                currentYInter = a1 * currentXInter + b1;
            } else {
                // Coeficiente angular
                double a2 = (Vertex_GetY(p2) - Vertex_GetY(p1)) / (Vertex_GetX(p2) - Vertex_GetX(p1));

                // Termo independente
                double b2 = Vertex_GetY(p1) - a2 * Vertex_GetX(p1);

                if (vertical)
                    currentXInter = Vertex_GetX(p);
                else
                    currentXInter = (b2 - b1) / (a1 - a2);

                currentYInter = a2 * currentXInter + b2;
            }
                

            // Distância entre o ponto de intersecção e o ponto central
            double distInter = euclideanDistance(x, y, currentXInter, currentYInter);

            // Segmento do ponto analisado não está à frente
            if (distInter < dist || fabs(distInter - dist) < 0.000001) {
                inFront = false;
                break;
            } else if (distInter >= dist && (minDist == -1 || distInter <= minDist)) {
                minDist = distInter;
                closestSegmentBehind = currentSegment;
                xInter = currentXInter;
                yInter = currentYInter;
            }
        }

        if (inFront) {
            // Se o segmento estiver na frente de todos os ativos
            if (!Vertex_IsStarting(p)) {
                // Se o ponto for final
                double xBiombo = Segment_GetXBiombo(s);
                double yBiombo = Segment_GetYBiombo(s);
                // Colocar luz a partir do biombo do segmento até o ponto
                putSVGTriangle(outputFile, xBiombo, yBiombo, x, y, Vertex_GetX(p), Vertex_GetY(p));
                if (closestSegmentBehind != NULL) {
                    // Se houver um segmento atrás
                    // Definir o biombo deste segmento como o ponto de intersecção
                    // entre ele e a reta
                    Segment_SetXBiombo(closestSegmentBehind, xInter);
                    Segment_SetYBiombo(closestSegmentBehind, yInter);
                }
            } else if (closestSegmentBehind != NULL) {
                // Se o ponto for inicial e houver um segmento atrás
                double xBiombo = Segment_GetXBiombo(closestSegmentBehind);
                double yBiombo = Segment_GetYBiombo(closestSegmentBehind);
                // Colocar luz a partir do biombo deste segmento até o ponto de intersecção
                // entre ele e a reta
                putSVGTriangle(outputFile, xBiombo, yBiombo, x, y, xInter, yInter);
            }
        }

        if (Vertex_IsStarting(p)) {
            RBTree_Insert(activeSegments, Segment_GetKey(s), s);            
        } else {
            RBTree_Remove(activeSegments, Segment_GetKey(s));
            //StList_Remove(activeSegments, compareAddr, s);
        }
    }

    putSVGBomb(outputFile, x, y);

    free(vertexes);

    RBTree_Destroy(activeSegments, Segment_Destroy);
    for (int i = 0; i < nSegments; i++) {
        Segment_Destroy(segments[i]);
    }
    free(segments);

    return true;
}
