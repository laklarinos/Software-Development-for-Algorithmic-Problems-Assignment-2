#ifndef CURVE
#define CURVE
#include "../helpers/includes.h"

class curve
{
private:
    int numPoints;
    int dimensions;
    string key;
    vector<pair<int, double>> arrayOfPoints;

public:
    string getKey();
    vector<pair<int, double>> *getVector();
    int getDimensions();
    curve(vector<pair<int,double>> pVector);
    curve(vector<pair<int,double>> pVector, string key);
    curve();
    ~curve();
};
#endif