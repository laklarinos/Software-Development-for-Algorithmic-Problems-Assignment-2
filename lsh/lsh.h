#ifndef LSH
#define LSH
#include "others.h"
int lsh(char* inputFile, char* queryFile, char* outputFile, lshConstants& lshCon, char* frechet, char* metric = "", double delta = 0.0);
#endif