#ifndef LSH
#define LSH
#include "../helpers/helpers.h"
#include "lshhashtable.h"
int lsh(char *inputFile, char *queryFile, char *outputFile, lshConstants &lshCon, char *frechet, char *metric = "", double delta = 0.0);
#endif