#ifndef OTHERS
#define OTHERS

#include "../point.h"

using namespace std;

typedef struct kNearest
{
    vector<point *> nearestPoints;
    vector<double> dist;
    int size;
    vector<chrono::duration<double, std::micro>> vecOfTimes;
} kNearest;

typedef struct vAndT
{
    std::vector<int> vVector;
    float t;
} vAndT;

typedef struct lshConstants
{
    int w;
    int k;
    int L;
} lshConstants;

bool checkCommandLineArguments(int argcInt, char *argvArray[]);
int euclideanRemainder(int a, int b);
int myPow(int x, int p);
double calculateDistance(point *point1, point *point2);
void initKNearest(int k, kNearest *list);
void sortNearest(kNearest *list);
bool zeroOrOne(int numGiven);
int binToDec(vector<int> &binVector);
void findVectorsOfHamDist(vector<int> vecGiven, int hamDist, vector<vector<int>> *vecToFill, int index);
void decToBin(vector<int> *vecGiven, int numBits, int dec);
#endif