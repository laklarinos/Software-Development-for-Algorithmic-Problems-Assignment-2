#ifndef HEL
#define HEL

class hashTable;
class hashTableCube;

#include "../importantStructs/structs.h"
#include "../lsh/lshhashtable.h"
#include "../hypercube/cubehashtable.h"
#include "includes.h"
#include "../importantStructs/point.h"
#include "../Fred-master/include/frechet.hpp"

static vector<vector<float>> DEFAULT_VECTOR;

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
vector<float> snap(int dim, vector<pair<float, float>> &mapCurve, double delta, pair<float, float> txAndTy);
float DFD(vector<float> curve1, vector<float> curve2, int meanFrechet = 0, vector<vector<float>> &vecToFill = DEFAULT_VECTOR);
float CFD(vector<float> curve1, vector<float> curve2);
float euclDistanceOf2Points(float x1, float x2, float y1, float y2);
Curve toRepoCurve(vector<float> myCurve);
void printToFile(char *outputFile,
                 char *algorithm,
                 vector<kNearest> &list,
                 vector<kNearest> &listTrue,
                 vector<hashTable *> &arrayOfHashTables,
                 int numberOfNN,
                 vector<point> &arrayOfCurves1DQ,
                 vector<vector<vector<float>>> const &arrayOfCurves1DQSnapped = {});
vector<float> meanCurve(vector<vector<float>> &vecOfCurves);
void meanFrechet(vector<float> &curve1, vector<float> &curve2, vector<float> *vecToFill);
vector<pair<int, int>> getOptimalTraversal(vector<vector<float>> vecOfDFD, vector<float> &curve1, vector<float> &curve2);

#endif