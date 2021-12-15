#ifndef STR
#define STR
#include "../helpers/includes.h"
#include "point.h"
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

typedef struct cubeConstants
{
    int probes;
    int M;
    int L;
    int k;
    int w;
    
}cubeConstants;
#endif