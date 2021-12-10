#ifndef PARS_INIT
#define PARS_INIT
#include "includes.h"
#include "others.h"
using namespace std;
int parsInit(char *inputFile, vector<point> &vecPoints, int *numLinesReturn, int *numOfElements); // opens inputFile nad initializes the 2d array...
void clean(int **);
#endif
