#ifndef HEL
#define HEL
#include "includes.h"
#include "../importantStructs/point.h"
#include "../importantStructs/structs.h"
int euclideanRemainder(int a, int b);
int myPow(int x, int p);
double euclDistance(point *point1, point *point2);
bool zeroOrOne(int numGiven);
int binToDec(vector<int> &binVector);
void findVectorsOfHamDist(vector<int> vecGiven, int hamDist, vector<vector<int>> *vecToFill, int index);
void decToBin(vector<int> *vecGiven, int numBits, int dec);
int parsInit(char *inputFile, vector<point> &vecPoints, int *numLinesReturn, int *numOfElements); // opens inputFile nad initializes the 2d array...
bool checkCommandLineArguments(int argcInt, char *argvArray[]);
void initKNearest(int k, kNearest *list);
void sortNearest(kNearest *list);
vector<float> snap(int dim, map<float,float>& mapCurve, double delta, pair<float,float> txAndTy);
float DFD(vector<float> curve1, vector<float> curve2);
float euclDistanceOf2Points(float x1, float x2, float y1, float y2);
#endif