#ifndef QUERY_H
#define QUERY_H

#include <stdio.h>
#include <stdbool.h>
#include "modules/aux/distance.h"
#include "modules/aux/heapsort.h"
#include "modules/aux/vertex.h"
#include "modules/data_structures/static_list.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"
#include "modules/sig/geometry.h"
#include "modules/sig/object.h"
#include "modules/sig/svg.h"
#include "lists.h"
#include "data.h"
#include "file_util.h"

// T1
bool Query_Overlaps(FILE *txtFile, FILE *outputFile, char idA[], char idB[]);

bool Query_Inside(FILE *txtFile, FILE *outputFile, char id[], double x, double y);

bool Query_Distance(FILE *txtFile, FILE *outputFile, char j[], char k[]);

bool Query_Bb(FILE *txtFile, FILE *outputFile, char outputDir[], char svgFileName[], char suffix[], char color[]);
/* ------------------------*/

// T2
bool Query_Dq(FILE *txtFile, char metric[], char id[], double dist);

bool Query_Del(FILE *txtFile, char id[]);

bool Query_Cbq(FILE *txtFile, double x, double y, double r, char cStrk[]);

bool Query_Crd(FILE *txtFile, char id[]);

bool Query_Trns(FILE *txtFile, double x, double y, double w, double h, double dx, double dy);
/* ------------------------*/

// T3
bool Query_Brl(FILE *outputFile, double x, double y);

bool Query_Fi(FILE *txtFile, FILE *outputFile, double x, double y, int ns, double r);

bool Query_Fh(FILE *txtFile, FILE *outputFile, char signal, int k, char cep[], char face, double num);

bool Query_Fs(FILE *txtFile, FILE *outputFile, int k, char cep[], char face, double num);
/* ------------------------*/

#endif