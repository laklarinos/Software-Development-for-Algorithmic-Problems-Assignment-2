#ifndef OTHERS
#define OTHERS

#include "includes.h"

using namespace std;

class point
{
private:
    int dimensions;

public:
    std::vector<int> pVector;
    std::vector<int> *getVector();
    int getDimensions();
    point(std::vector<int> pVector);
    point();
    ~point();
};

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
void findVectorsOfHamDist(vector<int> vecGiven, int hamDist, vector<vector<int>> *vecToFill, int index);
int binToDec(vector<int> &binVector);
bool zeroOrOne(int numGiven);
void decToBin(vector<int> *vecGiven, int numBits, int dec);
int euclideanRemainder(int a, int b);
int euclideanRemainder(int a, unsigned int b);
int myPow(int x, int p);
double calculateDistance(point *point1, point *point2);
void initKNearest(int k, kNearest *list);
void sortNearest(kNearest *list);
#endif