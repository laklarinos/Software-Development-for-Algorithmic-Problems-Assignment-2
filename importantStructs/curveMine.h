#ifndef CURVE
#define CURVE
#include "../helpers/includes.h"

class curveMine
{
private:
    int numPoints;
    int dimensions;
    string key;

public:
    vector<pair<float, float>> arrayOfPoints;
    string getKey();
    vector<pair<float, float>> *getVector();
    int getDimensions();
    curveMine(vector<pair<float, float>> pVector);
    curveMine(vector<pair<float, float>> pVector, string key);
    curveMine();
    ~curveMine();
};
#endif